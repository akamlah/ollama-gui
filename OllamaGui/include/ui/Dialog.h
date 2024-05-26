#ifndef OLLAMAGUI_UI_Dialog_H
#define OLLAMAGUI_UI_Dialog_H

#include <QDialog>
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QPushButton>


namespace Ui {
    class Dialog;
}

class Dialog: public QDialog
{
    Q_OBJECT

public:

    explicit Dialog(QString message, QWidget *parent = nullptr);
    virtual ~Dialog();

signals:
    void confirmed_signal();
    void cancelled_signal();

private:
    Ui::Dialog *_ui;

};
// }
#endif // OLLAMAGUI_UI_Dialog_H
