#include <ui/Dialog.h>
#include "./ui_dialog.h"


Dialog::Dialog(QString message, QWidget *parent) 
    : QDialog(parent)
    , _ui(new Ui::Dialog)

{
    _ui->setupUi(this);
    _ui->MessageLabel->setText(message);
    connect(_ui->OKButton, &QPushButton::clicked, this, [this]() {
        emit confirmed_signal();
    });
    connect(_ui->CancelButton, &QPushButton::clicked, this, [this]() {
        emit cancelled_signal();

    });
}

Dialog::~Dialog() {   }
