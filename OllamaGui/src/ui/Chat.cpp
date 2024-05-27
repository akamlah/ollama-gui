#include <ui/Chat.h>
#include "./ui_chat.h"
#include <ui/Dialog.h>

Chat::Chat(QString model, QWidget *parent) 
    :  QWidget(parent)
    , _parent(parent) 
    , _ui(new Ui::Chat)
    , _model_tag(model)
    , _doc(new QTextDocument(this))
    , _cursor(_cursor = new QTextCursor(_doc))
    , _network_manager(new QNetworkAccessManager(this))
{
    _ui->setupUi(this);
    parse_tag();
    _ui->MessageDisplay->setDocument(_doc);
    _ui->MessageDisplay->setReadOnly(true);
    _ui->PromptEditor->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    _ui->CheckBoxStream->setChecked(true);
    _options.StreamEnabled = true;

    connect(_ui->SendPromptButton, &QPushButton::clicked,
        this, [this]{ Chat::send_prompt_slot(); });

    QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+S")
        , _ui->PromptEditor);
    connect(shortcut, SIGNAL(activated()),
        this, SLOT(send_prompt_slot()));

    // [ ! ] make also ollama actually unload the model from ram
    connect(_ui->DisconnectButton, &QPushButton::clicked, 
        this, &Chat::confirm_disconnect_slot);

    // do this better
    _conversations.push_back("conv 1");
    _ui->ConversationsListWidget->addItem("Conv " 
        +  QString::fromUtf8(std::to_string(_conversations.size()).c_str()));
    
    load_model();

    connect(_ui->CheckBoxStream, &QCheckBox::checkStateChanged, this, 
        [this]() {
            _options.StreamEnabled = (_ui->CheckBoxStream->isChecked()) ? true : false;
        }
    );
}

Chat::~Chat() {
    qDebug() << "----- DESTROY";
    delete _ui;
}

void Chat::parse_tag() {
    if (_model_tag.isEmpty() || _model_tag.isNull())
        _model_tag = "Model";
    auto split_tag = _model_tag.split(u':');
    if (split_tag.length() > 0 && !split_tag[0].isEmpty() && !split_tag[0].isNull())
        _model_name = split_tag[0];
    else
        _model_name = _model_tag;
}

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
    dialog->show();
}

void Chat::wrap_set_enabled_send_button(bool setEnabled) {
    if (setEnabled) {
        _ui->SendPromptButton->setEnabled(true);
        _ui->SendPromptButton->setText("Send Prompt");
    }
    else {
        _ui->SendPromptButton->setEnabled(false);
        _ui->SendPromptButton->setText("Loading Reply ... ");
    }
}

void Chat::load_model() {
    wrap_set_enabled_send_button(false);
    _ui->SendPromptButton->setText("Loading Model");

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(Api::Endpoints::get_endpoints()->api_urls_post.generate_url);

    QJsonObject obj;
    obj.insert("model", QJsonValue::fromVariant(_model_tag));
    obj.insert("prompt", QJsonValue::fromVariant(""));
    obj.insert("stream", QJsonValue::fromVariant(false));
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    QNetworkReply *reply = _network_manager->post(request, data);
    QObject::connect(reply, &QNetworkReply::readyRead, this, [reply, this]() {
        // ... do something if no connection
    });
    QObject::connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        reply->deleteLater();
        wrap_set_enabled_send_button(true);
    });
}

void Chat::send_prompt_slot()
{
    if (_ui->SendPromptButton->isEnabled() == false) {
        return;
    }

    // delete trailing & leading newlines [ ! ]
    QString prompt = (_ui->PromptEditor->toPlainText());

    // add_message_item(MessageWidget::Role::FromUser, "You", prompt);

    // QTextBlockFormat format;
    // format.setBackground(Qt::green);
    _cursor->insertText("you");
    _cursor->insertText("\r\n");

    // cursor->insertBlock(format);
    _cursor->insertText(prompt);
    _cursor->insertText("\r\n");

    _qas.push_back(prompt);
    _ui->PromptEditor->setPlainText("");
    wrap_set_enabled_send_button(false);

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
    // qDebug() << doc;
    
    QNetworkReply *reply = _network_manager->post(request, data);

    _cursor->insertText(_model_name);
    _cursor->insertText("\r\n");

    QObject::connect(reply, &QNetworkReply::readyRead, this, [reply, this]() {
        while(reply->bytesAvailable()) {
            QByteArray response = reply->read(reply->bytesAvailable());
            QJsonObject json_obj = QJsonDocument::fromJson(response).object();
            auto model_answer = json_obj.value("response").toString();

            _cursor->insertText(model_answer);
        }
    });

    QObject::connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        _cursor->insertText("\r\n");
        reply->deleteLater();
        wrap_set_enabled_send_button(true);
    });

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
    qDebug() << doc;
    
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
            qDebug() << "Error:" << err;
        }
        reply->deleteLater();
    });
    // wait for model to answer and then reset the button
}

// void Chat::add_message_item(MessageWidget::Role role, QString sender, QString content) {
//     QListWidgetItem *msg_item = new QListWidgetItem();
//     MessageWidget *msg_widget = new MessageWidget(role, sender, content, this);
//     msg_item->setSizeHint((msg_widget->minimumSizeHint()));
//     _ui->MessageDisplayListWidget->addItem(msg_item);
//     _ui->MessageDisplayListWidget->setItemWidget(msg_item, msg_widget);
// }


// Chat::MessageWidget::MessageWidget(MessageWidget::Role role, QString sender, QString content, QWidget *parent) 
//     :  QWidget(parent)
// {
//     Qt::Alignment alignment = (role == MessageWidget::FromUser) ? Qt::AlignRight : Qt::AlignLeft;
//     Qt::LayoutDirection direction = (role == MessageWidget::FromUser) ? Qt::RightToLeft : Qt::LeftToRight;
//     QString color = (role == MessageWidget::FromModel) ? "#AC9BEF" : "#7DC8B1";

//     QLabel *senderLabel = new QLabel(this);
//     senderLabel->setText(sender);
//     senderLabel->setAlignment(alignment);
//     senderLabel->setLayoutDirection(direction);
//     senderLabel->setStyleSheet("color:" + color + "; margin-bottmo 5px; margin-top: 5px;");
//     senderLabel->setWordWrap(true);

//     QFont font_sender = senderLabel->font();
//     font_sender.setBold(true);
//     senderLabel->setFont(font_sender);

//     _contentLabel = new QLabel(this);
//     _contentLabel->setText(content);
//     _contentLabel->setAlignment(alignment);
//     _contentLabel->setLayoutDirection(direction);
//     _contentLabel->setStyleSheet(" color: #d0d0d0;");
//     _contentLabel->setWordWrap(true);

//     QGridLayout *layout = new QGridLayout(this);
//     layout->addWidget(senderLabel, 0, 0);
//     layout->addWidget(_contentLabel, 1, 0);
//     layout->setRowStretch(0, 0);
//     layout->setRowStretch(1, 1);
//     this->setLayout(layout);
// }


    // QNetworkReply *reply = _network_manager->post(request, data);
    // QObject::connect(reply, &QNetworkReply::finished, this, [reply, this]() {
    //     QRestReply restReply(reply);
    //     QByteArray response = reply->readAll();
    //     if (restReply.isSuccess() && !response.isEmpty()) {
    //         QJsonObject json_obj = QJsonDocument::fromJson(response).object();
    //         auto model_answer = json_obj.value("response").toString();
    //         if (!model_answer.isEmpty() && !model_answer.isNull()) {
    //             // auto mdel_answer_split = model_answer.split(u'\n');
    //             // if (!mdel_answer_split.isEmpty()) {
    //             //     for (auto line: mdel_answer_split) {
    //             //         _ui->MessageDisplayListWidget->addItem(line);
    //             //     }
    //             // }
    //             auto name_parts = _model.split(u':');
    //             // _ui->MessageDisplayListWidget->addItem(name_parts[0] + "\n" + model_answer);
    //             add_message_item(MessageWidget::Role::FromModel, name_parts[0], model_answer);

    //             _qas.push_back(model_answer);
    //             if (_qas.size() % 6 == 0) {
    //                 get_title();
    //             }
    //         }
    //         _ui->SendPromptButton->setEnabled(true);
    //     }
    //     else{
    //         QString err = reply->errorString();
    //         _ui->MessageDisplayListWidget->addItem("An error occurred");
    //         _ui->MessageDisplayListWidget->addItem(err);
    //         qDebug() << "Error:" << err;
    //     }
    //     reply->deleteLater();
    // });
    // wait for model to answer and then reset the button



    // QListWidgetItem *msg_item = new QListWidgetItem();
    // MessageWidget *msg_widget = new MessageWidget(MessageWidget::Role::FromModel, _model, "", this);
    // msg_item->setSizeHint((msg_widget->minimumSizeHint()));
    // _ui->MessageDisplayListWidget->addItem(msg_item);
    // _ui->MessageDisplayListWidget->setItemWidget(msg_item, msg_widget);

    // _ui->MessageDisplayListWidget->setWordWrap(true);
    // _ui->MessageDisplayListWidget->setAlternatingRowColors(true);
    // // _ui->MessageDisplayListWidget->setAutoScroll(false);
    // _ui->MessageDisplayListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // _ui->MessageDisplayListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);