#include "OllamaGui.h"

OllamaGui::OllamaGui(QWidget *parent) : QWidget(parent) {

    this->setStyleSheet("background-color: #202020;");

    m_button = new QPushButton("Close", this);
    m_button->setGeometry(10, 10, 80, 30);
    m_button->setStyleSheet("color: #ffffff;");

    connect(m_button, SIGNAL (clicked()), QApplication::instance(), SLOT (quit()));

}

OllamaGui::~OllamaGui() {
    delete(m_button); // ?
}