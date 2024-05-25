#ifndef OLLAMAGUI_UI_Chat_H
#define OLLAMAGUI_UI_Chat_H

#include <QWidget>
#include <QJsonObject>
#include <vector>
#include <QWidget>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRestReply>
#include <QNetworkAccessManager>
#include <Api.h>

namespace Ui {
    class Chat;
}

class Chat: public QWidget
{
    Q_OBJECT

public:

    Chat(QString model, QWidget *parent = nullptr);
    ~Chat();

private:
    QWidget *_parent;
    Ui::Chat *_ui;
    QString _model;
    std::vector<QString> _conversations;
    std::vector<QString> _qas;
    QNetworkAccessManager *_network_manager;

    void get_title();

private slots:
    void send_prompt();

signals:
    void close_conversation_request_signal();

};

#endif // OLLAMAGUI_UI_Chat_H
