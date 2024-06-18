#include <ui/Chat.h>
#include "./ui_chat.h"
#include <ui/Dialog.h>

const Chat::MessageHtmlStrings Chat::_html = Chat::MessageHtmlStrings();

Chat::Chat(QString model, QWidget *parent) 
    :  QWidget(parent)
    , _parent(parent) 
    , _ui(new Ui::Chat)
    , _model_tag(model)
    , _doc(new QTextDocument(this))
    , _cursor(_cursor = new QTextCursor(_doc))
    , _network_manager(new QNetworkAccessManager(this))
{
    // setup ui 
    _ui->setupUi(this);
    this->setObjectName("chatt");
    
    _ui->MessageDisplay->setDocument(_doc);
    _ui->MessageDisplay->setReadOnly(true);

    _ui->WordStreamExplanationLabel->setWordWrap(true);
    _ui->PromptEditor->setFocusPolicy(Qt::FocusPolicy::ClickFocus);

    // parse model name and load model into memory
    this->parse_tag();
    this->load_model_request();

    // set default states on chat tab opening
    _ui->CheckBoxContext->setChecked(true);
    _ui->CheckBoxStream->setChecked(true);
    _options.StreamEnabled = true;

    // [ ! ] change later when more formats supported
    _ui->PlainTextExplanationLabel->setWordWrap(true);
    _ui->CheckBoxPlainText->setChecked(true);
    _ui->CheckBoxPlainText->setAttribute(Qt::WA_TransparentForMouseEvents);
    _ui->CheckBoxPlainText->setFocusPolicy(Qt::NoFocus);

    // Open a new conversation
    // [ ! ] do this better
    _conversations.push_back("conv 1");
    _ui->ConversationsListWidget->addItem("Conv " 
        +  QString::fromUtf8(std::to_string(_conversations.size()).c_str()));

    // configure send prompt area signals and slots
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+S"), _ui->PromptEditor); // fix this, doesn't work on macos
    connect(shortcut, SIGNAL(activated()), this, SLOT(send_prompt_slot()));
    connect(_ui->SendPromptButton, &QPushButton::clicked, this, [this]{ Chat::send_prompt_slot(); });

    // configure options area signals and slots
    // [ ! ] make also ollama actually unload the model from ram
    connect(_ui->DisconnectButton, &QPushButton::clicked, this, &Chat::confirm_disconnect_slot);
    connect(_ui->CheckBoxStream, &QCheckBox::checkStateChanged, this, 
        [this]() {
            _options.StreamEnabled = (_ui->CheckBoxStream->isChecked()) ? true : false;
        }
    );
}

Chat::~Chat() {
    qDebug() << "Chat destructor - model: " << _model_name;
    delete _ui;
}


// ----------------------------------------------------------------------------------
// Slots
// ----------------------------------------------------------------------------------


/// @brief Asks to confirm disconnect from model
void Chat::confirm_disconnect_slot() {
    QString message = "Confirm disconnection from " + _model_name + " (" 
        + _model_tag + ")?" + "\n" + "All cached conversation will be lost";
    Dialog * dialog = new Dialog(message, this);
    // dialog->setMinimumSize(_parent->minimumSize());
    connect(dialog, &Dialog::confirmed_signal, this, [this, dialog]() {
        dialog->close();
        emit close_conversation_request_signal();
        dialog->deleteLater();
    });
    connect(dialog, &Dialog::cancelled_signal, this, [this, dialog]() {
        dialog->close();
        dialog->deleteLater();
    });
    dialog->exec();
}

/// @brief Slot called on prompt enter. Makes an api call to generete
/// a reply and prints that to the screen in a fromatted way, and adds the 
/// user's prompt above it.
void Chat::send_prompt_slot()
{
    if (!_ui->SendPromptButton->isEnabled()) {
        return;
    }
    QString prompt = (_ui->PromptEditor->toPlainText());
    this->wrap_set_enabled_send_button(false);
    this->flush_prompt_editor_to_message_display(prompt);
    _ui->MessageDisplay->verticalScrollBar()->setSliderPosition(_ui->MessageDisplay->verticalScrollBar()->maximum());

    // add model name to display
    _cursor->insertHtml(Chat::_html.model_name % _model_name);
    _cursor->insertHtml(Chat::_html.reset);
    _cursor->insertText("\r\n");
    _ui->MessageDisplay->verticalScrollBar()->setSliderPosition(_ui->MessageDisplay->verticalScrollBar()->maximum());

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(Api::Endpoints::get_endpoints()->api_urls_post.generate_url);

    QJsonObject obj;
    obj.insert("model", QJsonValue::fromVariant(_model_tag));
    obj.insert("prompt", QJsonValue::fromVariant(prompt));
    if (!_options.StreamEnabled)
        obj.insert("stream", QJsonValue::fromVariant(false));

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();    
    QNetworkReply *reply = _network_manager->post(request, data);
    QObject::connect(reply, &QNetworkReply::readyRead, this, [reply, this]() {
        while(reply->bytesAvailable()) {
            QByteArray response = reply->read(reply->bytesAvailable());
            qDebug() << "Chat::send_prompt_slot - Response:\r\n" << response;
            QJsonObject json_obj = QJsonDocument::fromJson(response).object();
            // qDebug() << "Chat::send_prompt_slot - Response:\r\n" << json_obj;
            auto model_answer = json_obj.value("response").toString();
            if (_options.StreamEnabled)
                _cursor->insertText(model_answer);
            else
                _cursor->insertMarkdown(model_answer);
            _ui->MessageDisplay->verticalScrollBar()->setSliderPosition(_ui->MessageDisplay->verticalScrollBar()->maximum());
        }
        _ui->MessageDisplay->verticalScrollBar()->setSliderPosition(_ui->MessageDisplay->verticalScrollBar()->maximum());
    });

    QObject::connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        _cursor->insertHtml(Chat::_html.reset % "\n");
        _cursor->insertText("\r\n");
        _cursor->insertText("\r\n");
        reply->deleteLater();
        this->wrap_set_enabled_send_button(true);
        _ui->MessageDisplay->verticalScrollBar()->setSliderPosition(_ui->MessageDisplay->verticalScrollBar()->maximum());
    });

}


// ----------------------------------------------------------------------------------
// Private member functions
// ----------------------------------------------------------------------------------


void Chat::parse_tag() {
    if (_model_tag.isEmpty() || _model_tag.isNull())
        _model_tag = "Model";
    auto split_tag = _model_tag.split(u':');
    if (split_tag.length() > 0 && !split_tag[0].isEmpty() && !split_tag[0].isNull())
        _model_name = split_tag[0];
    else
        _model_name = _model_tag;
}


void Chat::load_model_request() {
    this->wrap_set_enabled_send_button(false);
    _ui->ShortctuLabel->setText("Loading Model...");

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(Api::Endpoints::get_endpoints()->api_urls_post.generate_url);

    QJsonObject obj;
    obj.insert("model", QJsonValue::fromVariant(_model_tag));
    obj.insert("prompt", QJsonValue::fromVariant(""));
    obj.insert("stream", QJsonValue::fromVariant(false));
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    // Do something if no connection [ ! ]
    QNetworkReply *reply = _network_manager->post(request, data);
    QObject::connect(reply, &QNetworkReply::readyRead, this, [reply, this]() {
        // ... do some loading animation
    });
    QObject::connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        reply->deleteLater();
        // ... end loading animation
        this->wrap_set_enabled_send_button(true);
    });
    _cursor->insertHtml(Chat::_html.reset % "\n");
}


void Chat::flush_prompt_editor_to_message_display(const QString& prompt) {
    // put prompt to display
    // delete trailing & leading newlines [ ! ]
    _cursor->insertHtml(Chat::_html.user_name % QString("you") % Chat::_html.reset);
    _cursor->insertText(prompt);
    _cursor->insertHtml(Chat::_html.reset % "\n");
    _cursor->insertText("\r\n");
    _cursor->insertText("\r\n");
    _ui->PromptEditor->setPlainText(""); // reset prompt editor
}


void Chat::wrap_set_enabled_send_button(bool setEnabled) {
    if (setEnabled) {
        _ui->SendPromptButton->setEnabled(true);
        _ui->ShortctuLabel->setText("[Ctrl + S]/[Cmd + S] to send");

        // _ui->SendPromptButton->setText("Send Prompt");
    }
    else {
        _ui->SendPromptButton->setEnabled(false);
        _ui->ShortctuLabel->setText("Loading Reply ... ");

        // _ui->SendPromptButton->setText("Loading Reply ... ");
    }
}


void Chat::get_title() {

    QString prompt = ("Give a 3 word title to our conversation. Answer only with the title and nothing else.");

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(Api::Endpoints::get_endpoints()->api_urls_post.generate_url);

    QJsonObject obj;
    obj.insert("model", QJsonValue::fromVariant(_model_tag));
    obj.insert("prompt", QJsonValue::fromVariant(prompt));
    obj.insert("stream", QJsonValue::fromVariant(false));

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();
    qDebug() << "Chat::get_title Response:\r\n" << doc;
    
    QNetworkReply *reply = _network_manager->post(request, data);
    QObject::connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        QRestReply restReply(reply);
        QByteArray response = reply->readAll();
        if (restReply.isSuccess() && !response.isEmpty()) {
            QJsonObject json_obj = QJsonDocument::fromJson(response).object();
            auto model_answer = json_obj.value("response").toString();
            if (!model_answer.isEmpty() && !model_answer.isNull()) {
                _ui->ConversationsListWidget->item(_ui->ConversationsListWidget->count() - 1)->setText(model_answer);
            }
        }
        else{
            QString err = reply->errorString();
            qDebug() << "Chat::get_title - Error:" << err;
        }
        reply->deleteLater();
    });
    // wait for model to answer and then reset the button
}
