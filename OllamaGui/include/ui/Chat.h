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

private:

    typedef struct Options {
        bool StreamEnabled = false;
        bool AddDetails = false;
    } options;

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
    // initializer list
    QWidget *_parent;
    Ui::Chat *_ui;
    QString _model_tag;
    QTextDocument *_doc;
    QTextCursor* _cursor;
    QNetworkAccessManager *_network_manager;
    // other
    QString _model_name;
    options _options;

    std::vector<QString> _qas;
    std::vector<QString> _conversations;

    void parse_tag();
    void load_model();
    void wrap_set_enabled_send_button(bool setEnabled);
    void get_title();

    // void add_message_item(MessageWidget::Role role, QString sender, QString content);

private slots:
    void send_prompt_slot();
    void confirm_disconnect_slot();
    // void createMessageWidget(QWidget *widget, QString sender, QString content);

signals:
    void close_conversation_request_signal();




};

#endif // OLLAMAGUI_UI_Chat_H
