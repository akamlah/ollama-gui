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
#include <api/Api.h>

#include <QGridLayout>
#include <QtGui>

namespace Ui {
    class Chat;
}

class Chat: public QWidget
{
    Q_OBJECT

public:
    class MessageWidget: public QWidget {
    public:
        typedef enum role {
            FromUser,
            FromModel
        } Role ;
    public:
        MessageWidget(Role role, QString sender, QString content, QWidget *parent = nullptr);
        ~MessageWidget();
    };


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
    void add_message_item(MessageWidget::Role role, QString sender, QString content);

private slots:
    void send_prompt();
    // void createMessageWidget(QWidget *widget, QString sender, QString content);

signals:
    void close_conversation_request_signal();




};

#endif // OLLAMAGUI_UI_Chat_H
