#include <ui/SelectModel.h>
#include "./ui_selectmodel.h"
#include <api/Api.h>
#include <ui/Dialog.h>

// constructor
SelectModel::SelectModel(QWidget *parent) 
    : QWidget(parent)
    , _parent(parent)
    , _ui(new Ui::SelectModel)
    , _network_manager(new QNetworkAccessManager(this))
{
    _ui->setupUi(this);

    //later: "updateLabel signal with "setText" on url change
    // _ui->urlLabel->setText( "Currentnly connected to ollama server instance at: " +
    //     Api::Endpoints::get_endpoints()->get_base_url().toString() );
    // _ui->urlLabel->setWordWrap(true);
    _ui->currentlyLoadedLabel->setStyleSheet("font-weight: bold;");

    // _ui->ChangeUrlButton->setFixedSize(_ui->ChangeUrlButton->size());
    // _ui->refreshButton->setFixedSize(_ui->refreshButton->size());


    // [ ! ] Add management of case server not responding at first api call
    fetch_tags();

    // connect slots and signals
    QObject::connect( _ui->modelsList, &QListWidget::itemDoubleClicked,
        this, [this] { SelectModel::model_was_double_clicked_slot(); } );
    QObject::connect( this, &SelectModel::model_was_confirmed_signal,
        this, &SelectModel::model_was_selected_slot );
    QObject::connect( _ui->refreshButton, &QPushButton::clicked,
        this, &SelectModel::fetch_tags );
}

// destructor
SelectModel::~SelectModel() { delete _ui; }

/// @brief Creates a new dialog that asks to confirm to run the model selected
void SelectModel::model_was_double_clicked_slot() {
    auto model_selected = _ui->modelsList->selectedItems();
    int index = _ui->modelsList->row(model_selected[0]);
    QString model_name = _model_list.at(index);
    QString message = "Load " + model_name + " ?";
    Dialog * dialog = new Dialog(message, this);
    // dialog->setMinimumSize(_parent->minimumSize());
    connect(dialog, &Dialog::confirmed_signal, this, [this, dialog, model_name]() {
        dialog->close();
        emit model_was_confirmed_signal(model_name);
        dialog->deleteLater();
    });
    connect(dialog, &Dialog::cancelled_signal, this, [this, dialog]() {
        dialog->close();
        dialog->deleteLater();
    });
    dialog->show();
}

// Slot
void SelectModel::model_was_selected_slot(QString model_name) {
    emit model_was_selected_signal(model_name);
}

// private helper function
void SelectModel::display_tags() {
    if (_model_list.empty())
        return ;
    _ui->modelsList->clear();
    for (QString tag: _model_list)
        _ui->modelsList->addItem(tag);
}

// slot
void SelectModel::fetch_tags() {
    _model_list.clear();
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
                    QJsonArray models = json_obj.value("models").toArray();
                    for (int i = 0; i < models.size(); i++) {
                        QJsonObject model = models[i].toObject();
                        _model_list.push_back(model.value("name").toString());
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
        reply->deleteLater();
    });
}
