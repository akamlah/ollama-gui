#ifndef OLLAMAGUI_UI_MainWindow_H
#define OLLAMAGUI_UI_MainWindow_H

#include <QMainWindow>
#include <QPushButton>
#include <QProcess>
#include <QApplication>
#include <QStackedWidget>
#include <QGridLayout>
#include <QTabWidget>
#include <QFile>

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

public slots:

    void model_was_selected_slot(QString name);
    void close_conversation_request_slot();

private:

    QWidget *_central_widget;
    QStackedWidget *_stackedWidget;
    QTabWidget * _tabWidget;
    QPushButton *_nav_button;
    int _font_size;
    QBoxLayout * _header_layout;

private:

    void source_stylesheet();
    void setup_central_widget_navigation_area();
    void create_new_model_selection_view();
    void setup_zoomin_zoomout_shortcuts();

};

#endif // OLLAMAGUI_UI_MainWindow_H
