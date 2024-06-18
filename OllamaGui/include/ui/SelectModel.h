#ifndef OLLAMAGUI_UI_SelectModel_H
#define OLLAMAGUI_UI_SelectModel_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRestReply>
#include <QDialog>
#include <vector>

namespace Ui {
    class SelectModel;
}

class SelectModel: public QWidget
{
    Q_OBJECT

public:

    SelectModel(QWidget *parent);
    ~SelectModel();

signals:
    void model_was_confirmed_signal(QString model_name);
    void model_was_selected_signal(QString model_name);

private slots:
    void model_was_double_clicked_slot();
    void model_was_selected_slot(QString model_name);
    void fetch_tags();

private:
    Ui::SelectModel *_ui;
    QNetworkAccessManager _network_manager;
    std::vector<QString> _model_list;

    void display_tags();

};


#endif // OLLAMAGUI_UI_SelectModel_H
