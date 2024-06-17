#ifndef OLLAMAGUI_UI_SelectServer_H
#define OLLAMAGUI_UI_SelectServer_H

#include <QWidget>
#include <QTextEdit>
#include <api/Request.h>

#include <QNetworkAccessManager>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRestReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <vector>


namespace Ui {
    class SelectServer;
}

class SelectServer: public QWidget
{
    Q_OBJECT

public:

    SelectServer(QWidget *parent = nullptr);
    ~SelectServer();

private:

    Ui::SelectServer *_ui;
    QNetworkAccessManager * _network_manager;
    bool _started_editing;

signals:

    void server_was_selected_signal();

private:

    void try_fetch_tags();

};

#endif // OLLAMAGUI_UI_SelectServer_H
