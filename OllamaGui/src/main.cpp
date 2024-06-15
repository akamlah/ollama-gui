#include <iostream>
#include <QApplication>
#include <ui/MainWindow.h>

int main(int argc, char *argv[])
{
    try {
        QApplication q_app (argc, argv);
        MainWindow ollama_gui;
        ollama_gui.show();
        q_app.exec();
    } catch (std::exception e) {
        std::cerr << "An error occurred" << std::endl;
        std::cerr << e.what() << std::endl;
        return 1;
    }
    system("leaks ollamagui 1>&2");
    return 0;
}