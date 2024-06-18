#include <ui/SelectModel.h>
#include "./ui_selectmodel.h"
#include <api/Endpoints.h>
#include <ui/Dialog.h>

// constructor
SelectModel::SelectModel(QWidget *parent) 
    : QWidget(parent)
    , _ui(new Ui::SelectModel)
    , _network_manager(QNetworkAccessManager(this))
{
    _ui->setupUi(this);
    // [ ! ] Add management of case server not responding at first api call

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

// ----------------------------------------------------------------------------------
// Slots
// ----------------------------------------------------------------------------------

/// @brief Creates a new dialog that asks to confirm to run the model selected
void SelectModel::model_was_double_clicked_slot() {
    auto model_selected = _ui->modelsList->selectedItems();
    int index = _ui->modelsList->row(model_selected[0]);
    QString model_name = _model_list.at(index);
    QString message = "Load " + model_name + " ?";
    Dialog * dialog = new Dialog(message, this);
    connect(dialog, &Dialog::confirmed_signal, this, [this, dialog, model_name]() {
        dialog->close();
        emit model_was_confirmed_signal(model_name);
        dialog->deleteLater();
    });
    connect(dialog, &Dialog::cancelled_signal, this, [this, dialog]() {
        dialog->close();
        dialog->deleteLater();
    });
    dialog->exec();
}

// Slot
void SelectModel::model_was_selected_slot(QString model_name) {
    emit model_was_selected_signal(model_name);
}

// slot
void SelectModel::fetch_tags() {
    qDebug() << "fetching tags";
    _ui->currentlyLoadedLabel->setText("Loading...");
    _ui->clickToSelectLabel->setText("");
    _ui->refreshButton->setDisabled(true);
    _model_list.clear();
    QNetworkRequest request;
    request.setUrl(Api::Endpoints::get_endpoints()->api_urls_get.tags_url);
    QNetworkReply *reply = _network_manager.get(request);
    QObject::connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        QRestReply restReply(reply);
        if (restReply.isSuccess()) {
            _ui->clickToSelectLabel->setText("(Double click to select)");
            _ui->currentlyLoadedLabel->setText("Currently available models");
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
                else if (json_doc.isArray()) {
                    qDebug() << "Chat::fetch_tags - Error: tags json doc is array";
                    _ui->currentlyLoadedLabel->setText("An error occurred");
                }
                else {
                    qDebug() << "Chat::fetch_tags - Error: tags json doc neither array nor object";
                    _ui->currentlyLoadedLabel->setText("An error occurred");
                }
            }
            else {
                qDebug() << "Chat::fetch_tags - Error: empty response";
                _ui->currentlyLoadedLabel->setText("An error occurred");
            }
        }
        else {
            _ui->currentlyLoadedLabel->setText("No connection");
            qDebug() << "Chat::fetch_tags - Error: Ollama server not responding. Tags could not be fetched";

        }
        _ui->refreshButton->setDisabled(false);
        reply->deleteLater();
    });
}


// ----------------------------------------------------------------------------------
// Private member functions
// ----------------------------------------------------------------------------------

/// @brief prints model tags from list to ui list widget
void SelectModel::display_tags() {
    if (_model_list.empty())
        return ;
    _ui->modelsList->clear();
    for (QString tag: _model_list)
        _ui->modelsList->addItem(tag);
}
