#include <ui/Chat.h>
#include "./ui_chat.h"

Chat::MessageWidget::~MessageWidget() {}

Chat::Chat(QString model, QWidget *parent) 
    : _model(model)
    ,  QWidget(parent)
    , _parent(parent) 
    , _ui(new Ui::Chat)
    , _network_manager(new QNetworkAccessManager(this))
{
    _ui->setupUi(this);

    _ui->MessageDisplayListWidget->setWordWrap(true);
    _ui->MessageDisplayListWidget->setAlternatingRowColors(true);
    // _ui->MessageDisplayListWidget->setAutoScroll(false);
    _ui->MessageDisplayListWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _ui->MessageDisplayListWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    _ui->modelLabel->setText("");
    _ui->modelLabel->setStyleSheet("font-weight: bold;");

    _ui->PromptEditor->setFocusPolicy(Qt::FocusPolicy::ClickFocus);
    // _ui->SendPromptButton->setFixedSize(_ui->SendPromptButton->size());
    // _ui->closeConversationButton->setFixedSize(_ui->closeConversationButton->size());
    // _ui->NewConversationButton->setFixedSize(_ui->NewConversationButton->size());
    
    connect(_ui->SendPromptButton, &QPushButton::clicked,
        this, [this]{ Chat::send_prompt(); });
    connect(_ui->closeConversationButton, &QPushButton::clicked, 
        this, [this](){
            emit close_conversation_request_signal();
        });
    _conversations.push_back("conv 1");
    _ui->ConversationsListWidget->addItem("Conv " +  QString::fromUtf8(std::to_string(_conversations.size()).c_str()));
}

Chat::~Chat() {
    qDebug() << "----- DESTROY";
}

void Chat::send_prompt()
{
    QString prompt = (_ui->PromptEditor->toPlainText());

    add_message_item(MessageWidget::Role::FromUser, "You", prompt);

    _qas.push_back(prompt);
    _ui->PromptEditor->setPlainText("");
    _ui->SendPromptButton->setEnabled(false);

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(Api::Endpoints::get_endpoints()->api_urls_post.generate_url);

    QJsonObject obj;
    obj.insert("model", QJsonValue::fromVariant(_model));
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
                // auto mdel_answer_split = model_answer.split(u'\n');
                // if (!mdel_answer_split.isEmpty()) {
                //     for (auto line: mdel_answer_split) {
                //         _ui->MessageDisplayListWidget->addItem(line);
                //     }
                // }
                auto name_parts = _model.split(u':');
                // _ui->MessageDisplayListWidget->addItem(name_parts[0] + "\n" + model_answer);
                add_message_item(MessageWidget::Role::FromModel, name_parts[0], model_answer);

                _qas.push_back(model_answer);
                if (_qas.size() % 6 == 0) {
                    get_title();
                }
            }
            _ui->SendPromptButton->setEnabled(true);
        }
        else{
            QString err = reply->errorString();
            _ui->MessageDisplayListWidget->addItem("An error occurred");
            _ui->MessageDisplayListWidget->addItem(err);
            qDebug() << "Error:" << err;
        }
        reply->deleteLater();
    });
    // wait for model to answer and then reset the button
}

void Chat::get_title() {

    QString prompt = ("Give a 3 word title to our conversation. Answer only with the title and nothing else.");

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setUrl(Api::Endpoints::get_endpoints()->api_urls_post.generate_url);

    QJsonObject obj;
    obj.insert("model", QJsonValue::fromVariant(_model));
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

void Chat::add_message_item(MessageWidget::Role role, QString sender, QString content) {
    QListWidgetItem *msg_item = new QListWidgetItem();
    MessageWidget *msg_widget = new MessageWidget(role, sender, content, this);
    msg_item->setSizeHint((msg_widget->minimumSizeHint()));
    _ui->MessageDisplayListWidget->addItem(msg_item);
    _ui->MessageDisplayListWidget->setItemWidget(msg_item, msg_widget);
}


Chat::MessageWidget::MessageWidget(MessageWidget::Role role, QString sender, QString content, QWidget *parent) 
    :  QWidget(parent)
{
    Qt::Alignment alignment = (role == MessageWidget::FromUser) ? Qt::AlignRight : Qt::AlignLeft;
    Qt::LayoutDirection direction = (role == MessageWidget::FromUser) ? Qt::RightToLeft : Qt::LeftToRight;
    QString color = (role == MessageWidget::FromModel) ? "#AC9BEF" : "#7DC8B1";

    QLabel *senderLabel = new QLabel(this);
    senderLabel->setText(sender);
    senderLabel->setAlignment(alignment);
    senderLabel->setLayoutDirection(direction);
    senderLabel->setStyleSheet("color:" + color + "; margin-bottmo 5px; margin-top: 5px;");
    senderLabel->setWordWrap(true);

    QFont font_sender = senderLabel->font();
    font_sender.setBold(true);
    senderLabel->setFont(font_sender);

    QLabel *contentLabel = new QLabel(this);
    contentLabel->setText(content);
    contentLabel->setAlignment(alignment);
    contentLabel->setLayoutDirection(direction);
    contentLabel->setStyleSheet(" color: #d0d0d0;");
    contentLabel->setWordWrap(true);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(senderLabel, 0, 0);
    layout->addWidget(contentLabel, 1, 0);
    layout->setRowStretch(0, 0);
    layout->setRowStretch(1, 1);
    this->setLayout(layout);
}