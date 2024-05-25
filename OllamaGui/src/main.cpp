// stdlib
#include <iostream>
// Qt
#include <QApplication>
// local
// #include <ui/Widget.h>
#include <ui/MainWindow.h>
// #include <ui/RequestManager.h>

// class MainWindow;

int main(int argc, char *argv[])
{
    try {
        QApplication app (argc, argv);
        // OllGui::Ui::Widget ollama;
        MainWindow ollama;
        ollama.show();
        app.exec();
    } catch (std::exception e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    return 0;
}