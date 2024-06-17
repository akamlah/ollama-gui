#ifndef OLLAMAGUI_UI_MainWindow_H
#define OLLAMAGUI_UI_MainWindow_H

#include <QMainWindow>
#include <QPushButton>
// #include <QProcess>
#include <QApplication>
#include <QStackedWidget>
#include <QGridLayout>
#include <QTabWidget>
#include <QFile>
#include <QLabel>
#include <QScreen>

#include <unistd.h>

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE


class MainWindow: public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();

private slots:

    void server_was_selected_slot();
    void model_was_selected_slot(QString name);
    void close_conversation_request_slot();

private:

    QWidget *_central_widget;
    QStackedWidget *_stackedWidget;
    QTabWidget * _tabWidget;

    int _font_size;
    
    QBoxLayout * _header_layout;
    QLabel * _url_label;
    QPushButton *_nav_button;

    // for more readable indexing on stacked widget
    enum StackedViews {
        SelectServerView = 0,
        SelectModelView,
        ChatView,
    };

private:

    void source_stylesheet();
    void configure_header_layout();
    void configure_stacked_widget();
    void setup_zoomin_zoomout_shortcuts();

};

#endif // OLLAMAGUI_UI_MainWindow_H
