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
#include <QLabel>
#include <QTextDocument>
#include <QAction>

namespace Ui {
    class Chat;
}

class Chat: public QWidget
{
    Q_OBJECT

public:
    // class MessageWidget: public QWidget {
    // public:
    //     typedef enum role {
    //         FromUser,
    //         FromModel
    //     } Role ;
    // public:
    //     MessageWidget(Role role, QString sender, QString content, QWidget *parent = nullptr);
    //     ~MessageWidget();
    //     QLabel *_contentLabel;

    // };




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

    QTextDocument *_doc;
    QTextCursor* _cursor;

    void get_title();
    void load_model();
    void wrap_set_enabled_send_button(bool setEnabled);

    // void add_message_item(MessageWidget::Role role, QString sender, QString content);

private slots:
    void send_prompt();
    // void createMessageWidget(QWidget *widget, QString sender, QString content);

signals:
    void close_conversation_request_signal();




};

#endif // OLLAMAGUI_UI_Chat_H
