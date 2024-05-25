#include <ui/Chat.h>
#include "./ui_chat.h"

Chat::Chat(QString model, QWidget *parent) 
    : _model(model)
    , _parent(parent) 
    , _ui(new Ui::Chat)
{
    _ui->setupUi(this);
    // _ui->ActiveChatGroup->setTitle("Conversation");
    // _ui->modelLabel->setText("Model: " + model);
    _ui->modelLabel->setText("");
    _ui->modelLabel->setStyleSheet("font-weight: bold;");
    // _ui->ModelListWidgets->setH
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
    _ui->MessageDisplayListWidget->addItem((_ui->PromptEditor->toPlainText()));
    _ui->PromptEditor->setPlainText("");
    _ui->SendPromptButton->setEnabled(false);

    QJsonObject json_obj;

    // wait for model to answer and then reset the button
}
