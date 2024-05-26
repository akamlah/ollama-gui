#include <ui/Dialog.h>
// #include "./ui_dialog.h"


Dialog::Dialog(QString message, QWidget *parent) 
    : QDialog(parent)

{

    // _ui->setupUi(this);
    // qDebug() << "here";

    QGridLayout *layout = new QGridLayout(this);
    QPushButton *confirm_btn = new QPushButton(this);
    QPushButton *cancel_btn = new QPushButton(this);
    QLabel *label = new QLabel(this);

    // qDebug() << "here2";


    // dialog->setMinimumSize(parent->minimumSize());
    // dialog->move(this->geometry().center() - dialog->rect().center());

    label->setText(message);
    cancel_btn->setText("Cancel");
    confirm_btn->setText("OK");

    layout->addWidget(label, 0, 0, Qt::AlignCenter);
    layout->addWidget(cancel_btn, 1, 0);
    layout->addWidget(confirm_btn, 1, 1);
    layout->setRowStretch(0, 1);
    layout->setRowStretch(1, 1);

    connect(confirm_btn, &QPushButton::clicked, this, [this]() {
        emit confirmed_signal();
    });

    connect(cancel_btn, &QPushButton::clicked, this, [this]() {
        emit cancelled_signal();

    });

    // dialog->show();
}

Dialog::~Dialog() {   }
