#ifndef OLLAMAGIU_H
#define OLLAMAGIU_H

#include <QApplication>
#include <QWidget>
#include <QPushButton>

class OllamaGui: public QWidget {
    Q_OBJECT
    public:
        explicit OllamaGui(QWidget *parent = 0);
        virtual ~OllamaGui();

    // signals:
    // public slots:

    private:
    QPushButton *m_button;
    
};

#endif // OLLAMAGIU_H
