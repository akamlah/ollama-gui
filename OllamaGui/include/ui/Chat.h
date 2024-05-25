#ifndef OLLAMAGUI_UI_Chat_H
#define OLLAMAGUI_UI_Chat_H

#include <QWidget>
#include <QJsonObject>

namespace Ui {
    class Chat;
}

class Chat: public QWidget
{
    Q_OBJECT

public:

    Chat(QString model, QWidget *parent);
    ~Chat();

private:
    QWidget *_parent;
    Ui::Chat *_ui;
    QString _model;

private slots:
    void send_prompt();

signals:
    void change_model_request_signal();

};

#endif // OLLAMAGUI_UI_Chat_H
