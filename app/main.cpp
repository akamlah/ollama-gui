#include <QApplication>
#include "OllamaGui.h"

int main(int argc, char **argv)
{
    QApplication app (argc, argv);
    OllamaGui ollama;
    ollama.show();

    return app.exec();
}