#include <ui/SelectModel.h>
#include "./ui_selectmodel.h"
#include <Api.h>

SelectModel::SelectModel(QWidget *parent) 
    : QWidget(parent)
    , _parent(parent)
    , _ui(new Ui::SelectModel)
    , _network_manager(new QNetworkAccessManager(this))
{
    _ui->setupUi(this);

    //later: "updateLabel signal with "setText" on url change
    _ui->urlLabel->setText(
        "Currentnly connected to ollama server instance at: " +
        Api::Endpoints::get_endpoints()->get_base_url().toString()
    );

    fetch_tags();
    
    QObject::connect(
        _ui->modelsList, 
        &QListWidget::itemDoubleClicked,
        this,
        [this] { SelectModel::model_was_double_clicked_slot(this); }
    );
    
    QObject::connect(
        this, 
        &SelectModel::model_was_confirmed_signal,
        this,
        &SelectModel::model_was_selected_slot
    );

    QObject::connect(
        _ui->refreshButton, 
        &QPushButton::clicked,
        this,
        &SelectModel::fetch_tags
    );

}

SelectModel::~SelectModel() {

}

void SelectModel::model_was_double_clicked_slot(SelectModel* sm) {
    QDialog *dialog = new QDialog(this);
    QGridLayout *layout = new QGridLayout(dialog);
    QPushButton *confirm_btn = new QPushButton(dialog);
    QPushButton *cancel_btn = new QPushButton(dialog);
    QLabel *label = new QLabel(dialog);

    auto model_selected = sm->_ui->modelsList->selectedItems();
    int index = sm->_ui->modelsList->row(model_selected[0]);
    QString model_name = _model_list.at(index);

    dialog->setMinimumSize(_parent->minimumSize());
    // dialog->move(this->geometry().center() - dialog->rect().center());

    label->setText("Run model " + model_name + " ?");
    confirm_btn->setText("Confirm");
    cancel_btn->setText("Cancel");
    
    layout->addWidget(label, 0, 0);
    layout->addWidget(confirm_btn, 1, 0);
    layout->addWidget(cancel_btn, 1, 1);
    
    connect(confirm_btn, &QPushButton::clicked, this, [this, dialog, model_name]() {
        dialog->close();
        emit model_was_confirmed_signal(model_name);
    });

    connect(cancel_btn, &QPushButton::clicked, this, [this, dialog]() {
        dialog->close();
    });

    dialog->show();
}


void SelectModel::model_was_selected_slot(QString model_name) {
    emit model_was_selected_signal(model_name);
}

void SelectModel::display_tags() {
    qDebug() << "Displaying tags";
    if (!_model_list.empty()) {
        _ui->modelsList->clear();
        qDebug() << " tags not empty";
        for (QString tag: _model_list) {
            qDebug() << tag;
            // _ui->modelsTable->insertRow(_ui->modelsTable->rowCount());
            _ui->modelsList->addItem(tag);
        }
    }
}

void SelectModel::fetch_tags() {
    _model_list.clear();
    qDebug() << "Fetching tags";
    QNetworkRequest request;
    request.setUrl(Api::Endpoints::get_endpoints()->api_urls_get.tags_url);
    QNetworkReply *reply = _network_manager->get(request);
    QObject::connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        QRestReply restReply(reply);
        if (restReply.isSuccess()) {
            QByteArray response = reply->readAll();
            if (!response.isEmpty()) {
                QJsonDocument json_doc =  QJsonDocument::fromJson(response);
                if (json_doc.isObject()) {
                    QJsonObject json_obj = json_doc.object();
                    // qDebug() << "Json doc is object;";
                    // qDebug() << json_obj;
                    QJsonArray models = json_obj.value("models").toArray();
                    // qDebug() << models;
                    for (int i = 0; i < models.size(); i++) {
                        // qDebug() << "here";
                        QJsonObject model = models[i].toObject();
                        // qDebug() << model;
                        _model_list.push_back(model.value("name").toString());
                        // qDebug() << model.value("name").toString();
                        // _ui->ModelListWidget->addItem(model.value("name").toString());
                    }
                    display_tags();
                }
                else if (json_doc.isArray()) 
                    qDebug() << "Error: json doc is array";
                else
                    qDebug() << "Error: json doc neither array nor object";
            }
        }
        else
            qDebug() << "Error: request";
    });
}
