#include <ui/Chat.h>
#include "./ui_chat.h"

Chat::Chat(QWidget *parent) 
    : _parent(parent) 
    , _ui(new Ui::Chat)
{
    _ui->setupUi(this);
}

Chat::~Chat() {
}


// void MainWindow::on_SendPromptButton_clicked()
// {
//     // ui->MessageDisplayListWidget->addItem((ui->PromptEditor->toPlainText()));
//     // ui->PromptEditor->setPlainText("");
//     // ui->SendPromptButton->setEnabled(false);
//     // wait for model to answer and then reset the button
// }
