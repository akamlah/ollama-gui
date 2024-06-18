#include <ui/Chat.h>
#include "./ui_chat.h"
#include <ui/Dialog.h>

const Chat::MessageHtmlStrings Chat::_html = Chat::MessageHtmlStrings();

Chat::Chat(QString model, QWidget *parent) 
    :  QWidget(parent)
    , _ui(new Ui::Chat)
    , _model_tag(model)
    , _doc(QTextDocument(this))
    , _cursor(QTextCursor(&_doc))
    , _network_manager(QNetworkAccessManager(this))
    , _buffer(new QByteArray())
{
    // setup ui 
    _ui->setupUi(this);

    _ui->MessageDisplay->setDocument(&_doc);
    _ui->MessageDisplay->setReadOnly(true);

    _ui->PromptEditor->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    _ui->StopButton->hide();

    // parse model name and load model into memory
    this->parse_tag();
    this->load_model_request();


    // OPTIONS:


    // context
    _ui->ClearContextButton->setEnabled(false);
    _options.ContextEnabled = true;
    _ui->ContextExplanationLabel->setWordWrap(true);
    _ui->CheckBoxContext->setChecked(true);
    connect(_ui->CheckBoxContext, &QCheckBox::checkStateChanged, this, 
        [this](){ _options.ContextEnabled = (_ui->CheckBoxContext->isChecked()) ? true : false; } );

    // stream
    _options.StreamEnabled = true;
    _ui->WordStreamExplanationLabel->setWordWrap(true);
    _ui->CheckBoxStream->setChecked(true);
    connect(_ui->CheckBoxStream, &QCheckBox::checkStateChanged, this, 
        [this](){ _options.StreamEnabled = (_ui->CheckBoxStream->isChecked()) ? true : false; } );

    // details
    _options.AddDetails = false;
    connect(_ui->CheckBoxDetails, &QCheckBox::checkStateChanged, this, 
        [this](){ _options.AddDetails = (_ui->CheckBoxDetails->isChecked()) ? true : false; } );


    // Prompt options: [ ! ] change later when more formats supported
    _ui->PlainTextExplanationLabel->setWordWrap(true);
    _ui->CheckBoxPlainText->setChecked(true);
    _ui->CheckBoxPlainText->setAttribute(Qt::WA_TransparentForMouseEvents);
    _ui->CheckBoxPlainText->setFocusPolicy(Qt::NoFocus);

    // Open a new conversation
    // [ ! ] todo with database
    _conversations.push_back("conv 1");
    _ui->ConversationsListWidget->addItem("Conv " 
        +  QString::fromUtf8(std::to_string(_conversations.size()).c_str()));

    // configure send prompt area signals and slots
    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+S"), _ui->PromptEditor); // fix this, doesn't work on macos
    connect(shortcut, SIGNAL(activated()), this, SLOT(send_prompt_slot()));
    connect(_ui->SendPromptButton, &QPushButton::clicked, this, [this]{ Chat::send_prompt_slot(); });

    // configure options area signals and slots
    // [ ! ] make also ollama actually unload the model from ram in lesss than 5 minutes (default)
    connect(_ui->DisconnectButton, &QPushButton::clicked, this, &Chat::confirm_disconnect_slot);
    
    connect(_ui->ClearContextButton, &QPushButton::clicked, this, &Chat::clear_context_slot);

    connect(_ui->StopButton, &QPushButton::clicked, this, &Chat::stop_generating_slot);
}

Chat::~Chat() {
    qDebug() << "Chat destructor - model: " << _model_name;
    delete _buffer;
    delete _ui;
}


// ----------------------------------------------------------------------------------
// Slots
// ----------------------------------------------------------------------------------

/// @brief Stops reply generation
void Chat::stop_generating_slot() {
    emit stop_generating_signal();
}

/// @brief Asks to confirm disconnect from model
void Chat::confirm_disconnect_slot() {
    QString message = "Confirm disconnection from " + _model_name + " (" 
        + _model_tag + ")?" + "\n" + "All cached conversation will be lost";
    Dialog * dialog = new Dialog(message, this);
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

/// @brief Asks to confirm to clear conversation context cache
void Chat::clear_context_slot() {
    QString message = "Clear all cached context information for your current converation with "
        + _model_name + "?";
    Dialog * dialog = new Dialog(message, this);
    connect(dialog, &Dialog::confirmed_signal, this, [this, dialog]() {
        _ui->ClearContextButton->setEnabled(false);
        dialog->close();
        _context = QJsonArray();
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
/// user's prompt above it. If model still loading or prompt empty it just
/// returns
void Chat::send_prompt_slot()
{
    // do nothing if model or reply still loading
    if (_ui->SendPromptButton->isHidden()) {
        return ;
    }

    // read prompt and return if empty
    QString prompt = (_ui->PromptEditor->toPlainText());
    if (prompt.isEmpty()) {
        return ;
    }

    _buffer->clear();

    // print user name and prompt to display
    this->prompt_button_and_label_ready_to_send(false);
    this->flush_prompt_editor_to_message_display(prompt);
    _ui->MessageDisplay->verticalScrollBar()->setSliderPosition(_ui->MessageDisplay->verticalScrollBar()->maximum());

    // add model name to display
    _cursor.insertHtml(Chat::_html.model_name % _model_name);
    _cursor.insertHtml(Chat::_html.reset);
    _cursor.insertText("\r\n");
    _ui->MessageDisplay->verticalScrollBar()->setSliderPosition(_ui->MessageDisplay->verticalScrollBar()->maximum());

    // build json request and write it to json byte array
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(Api::Endpoints::get_endpoints()->api_urls_post.generate_url);
    QJsonObject obj;
    obj.insert("model", QJsonValue::fromVariant(_model_tag));
    obj.insert("prompt", QJsonValue::fromVariant(prompt));
    if (!_options.StreamEnabled)
        obj.insert("stream", QJsonValue::fromVariant(false));
    if (_options.ContextEnabled && !_context.isEmpty()) {
        // [ ? ]
        obj.insert("context", QJsonValue::fromVariant(_context));
        if (!_ui->ClearContextButton->isEnabled()) {
            _ui->ClearContextButton->setEnabled(true);
        }
    }

    QJsonDocument doc(obj);

    qDebug() << "Ready json doc with context:\n" << doc;

    QByteArray data = doc.toJson();

    // make request sending json byte array and save it into a reply object pointer
    QNetworkReply *reply = _network_manager.post(request, data);
    QJsonArray * tmp_context = new QJsonArray;

    // relply callback on bytes available
    QObject::connect(reply, &QNetworkReply::readyRead, this, [reply, tmp_context, this]() {
        while(reply->bytesAvailable()) {
            _buffer->append(reply->readLine());
            if (_buffer->isEmpty()) {
                qDebug() << "Chat::send_prompt_slot - Error: Byte array empty but bytes available";
                return ;
            }
            QJsonDocument json_doc = QJsonDocument::fromJson(*_buffer);
            if (json_doc.isObject()) {

                QJsonObject json_obj = QJsonDocument::fromJson(*_buffer).object();
                qDebug() << "Chat::send_prompt_slot - Response is object:\r\n" << json_obj;
                auto model_answer = json_obj.value("response").toString();
                if (_options.StreamEnabled)
                    _cursor.insertText(model_answer);
                else
                    _cursor.insertMarkdown(model_answer);
                if (json_obj.value("done") == true) {
                    qDebug() << "Chat::send_prompt_slot - Done -------";
                }
                else {
                    _buffer->clear();
                }
                if (_options.ContextEnabled) {
                    *tmp_context = json_obj.value("context").toArray();
                    qDebug() << "Chat::send_prompt_slot - context as json array:\r\n" << *tmp_context;
                }
                _ui->MessageDisplay->verticalScrollBar()->setSliderPosition(_ui->MessageDisplay->verticalScrollBar()->maximum());
            }
            else if (json_doc.isArray()) {
                QJsonArray json_array = QJsonDocument::fromJson(*_buffer).array();
                qDebug() << "Chat::send_prompt_slot - Response is array:\r\n" << json_array;
                _buffer->clear();
            }
            else {
                qDebug() << "Chat::send_prompt_slot - Json Object is null or empty. Wainting to append next bytestream";
                qDebug() << "Chat::send_prompt_slot - Bytes read so far:\r\n" << *_buffer;
            }
        }
    });

    QObject::connect(reply, &QNetworkReply::error, this, [reply, tmp_context, this]() {
        qDebug() << "Chat::send_prompt_slot - Network reply error";
        _buffer->clear();
        delete tmp_context;
    });

    QObject::connect(this, &Chat::stop_generating_signal, reply, [reply, tmp_context, this]() {
        qDebug() << "Chat::send_prompt_slot - Abort";
        _cursor.insertHtml(Chat::_html.reset % "\n");
        _cursor.insertText("\r\n");
        if (_options.ContextEnabled) {
            _cursor.insertHtml(Chat::_html.muted % "Stopped - not added to context");
        }
        else {
            _cursor.insertHtml(Chat::_html.muted % "Stopped");
        }
        *tmp_context = QJsonArray();
        reply->abort();
    });

    // relply callback on reply finished
    QObject::connect(reply, &QNetworkReply::finished, this, [reply, tmp_context, this]() {
        qDebug() << "Chat::send_prompt_slot - Finished";
        // reset ui
        _cursor.insertHtml(Chat::_html.reset % "\n");
        _cursor.insertText("\r\n");
        _cursor.insertText("\r\n");
        this->prompt_button_and_label_ready_to_send(true);
        _ui->MessageDisplay->verticalScrollBar()->setSliderPosition(_ui->MessageDisplay->verticalScrollBar()->maximum());
        // async delete
        reply->deleteLater();
        if (_options.ContextEnabled && !tmp_context->empty()) {
            // append or replace [ ? ]
            _context = *tmp_context;
        }
        delete tmp_context;
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
    _ui->SendPromptButton->show();
    _ui->SendPromptButton->setEnabled(false);
    _ui->StopButton->hide();
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
    QNetworkReply *reply = _network_manager.post(request, data);
    QObject::connect(reply, &QNetworkReply::readyRead, this, [reply, this]() {
        // ... do some loading animation
    });
    QObject::connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        reply->deleteLater();
        // ... end loading animation
        _ui->SendPromptButton->setEnabled(true);
        this->prompt_button_and_label_ready_to_send(true);
    });
    _cursor.insertHtml(Chat::_html.reset % "\n");
}


void Chat::flush_prompt_editor_to_message_display(const QString& prompt) {
    // put prompt to display
    // delete trailing & leading newlines [ ! ]
    _cursor.insertHtml(Chat::_html.user_name % QString("you") % Chat::_html.reset);
    _cursor.insertText(prompt);
    _cursor.insertHtml(Chat::_html.reset % "\n");
    _cursor.insertText("\r\n");
    _cursor.insertText("\r\n");
    _ui->PromptEditor->setPlainText(""); // reset prompt editor
}


void Chat::prompt_button_and_label_ready_to_send(bool ready) {
    if (ready) {
        _ui->SendPromptButton->show();
        _ui->StopButton->hide();
        _ui->ShortctuLabel->setText("[Ctrl + S]/[Cmd + S] to send");
    }
    else {
        _ui->SendPromptButton->hide();
        _ui->StopButton->show();
        _ui->ShortctuLabel->setText("Loading Reply ... ");
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
    
    QNetworkReply *reply = _network_manager.post(request, data);
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
