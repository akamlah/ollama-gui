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
#include <QGridLayout>
#include <QtGui>
#include <QLabel>
#include <QTextDocument>
#include <QAction>

#include <api/Endpoints.h>

namespace Ui {
    class Chat;
}

class Chat: public QWidget
{
    Q_OBJECT

private:

    typedef struct options {
        bool StreamEnabled = false;
        bool AddDetails = false;
    } Options;

    // rudimantal styling -> read from file later and find more elegant 
    // html formatting like putting things in divs instead of inserting 
    // 3 line breaks
    typedef struct messageHtmlStrings {
        QString model_name = "<p style=\"color: #7960ca; font-weight: bold\">";
        QString user_name = "<p style=\"color: #4caf91; font-weight: bold\">";
        QString reset = "</p><br>";
    } MessageHtmlStrings;

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

    Options _options;
    static const MessageHtmlStrings _html;

    std::vector<QString> _qas;
    std::vector<QString> _conversations;

private slots:
    void send_prompt_slot();
    void confirm_disconnect_slot();
    // void createMessageWidget(QWidget *widget, QString sender, QString content);

signals:
    void close_conversation_request_signal();

private:
    void parse_tag();
    void load_model_request();
    void wrap_set_enabled_send_button(bool setEnabled);
    void get_title();
    void flush_prompt_editor_to_message_display(const QString& prompt);
    // void add_message_item(MessageWidget::Role role, QString sender, QString content);




};

#endif // OLLAMAGUI_UI_Chat_H
// public:

    // class MessageElement: public QWidget {
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