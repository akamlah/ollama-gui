#ifndef OLLAMAGUI_UI_MainWindow_H
#define OLLAMAGUI_UI_MainWindow_H

#include <QMainWindow>
#include <QPushButton>
// #include <QStatusBar>
// #include <QMenuBar>
// #include <QTcpSocket>
// #include <QBoxLayout>
#include <QProcess>
#include <QApplication>
// #include <QRegularExpression>
// #include <QNetworkReply>
// #include <QObject>

// #include  <iostream>
// #include  <unistd.h>
// #include <QJsonDocument>
// #include <QJsonObject>
// #include <QJsonArray>
// #include <QRestReply>
#include <QStackedWidget>
#include <QComboBox>


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
    // class SelectModel;
// class Chat;
    
}
QT_END_NAMESPACE

class MainWindow: public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();

// private slots:
    // void api_get_tags_callback(QNetworkReply &reply);

public slots:
    void model_was_selected_slot(QString name);

private:
    // void api_get_tags();
    QStackedWidget *_stackedWidget;
    // Chat* chat;
    // SelectModel* select_model;


private:
    // Ui::MainWindow *_ui;
    // Ui::RequestManager *_request_manager;
    // QNetworkAccessManager *_network_manager;
    // QUrl _api_base_url;
};

#endif // OLLAMAGUI_UI_MainWindow_H
