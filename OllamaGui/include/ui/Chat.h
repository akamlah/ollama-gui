#ifndef OLLAMAGUI_UI_Chat_H
#define OLLAMAGUI_UI_Chat_H

#include <QWidget>

namespace Ui {
    class Chat;
}

class Chat: public QWidget
{
    Q_OBJECT

public:

    Chat(QWidget *parent = nullptr);
    ~Chat();

private:
    QWidget *_parent;
    Ui::Chat *_ui;

// private slots:

};

#endif // OLLAMAGUI_UI_Chat_H
