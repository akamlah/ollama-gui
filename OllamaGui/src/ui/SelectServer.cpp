#include <ui/SelectServer.h>
#include "./ui_selectserver.h"
#include <ui/Dialog.h>
#include <api/Endpoints.h>

SelectServer::SelectServer(QWidget *parent) 
    :  QWidget(parent)
    , _ui(new Ui::SelectServer)
    , _network_manager( new QNetworkAccessManager(this))
{
    // setup ui 
    _ui->setupUi(this);
    _ui->WelcomeLabel->setWordWrap(true);
    _ui->InfoLabel->setWordWrap(true);
    _ui->InfoLabel2->setWordWrap(true);
    _ui->InfoLabel3->setWordWrap(true);
    _ui->InfoLabel4->setWordWrap(true);
    _ui->CurrentUrlLabel->setWordWrap(true);
    _ui->InvalidUrlLabel->setWordWrap(true);
    _ui->CurrentUrlLabel->setWordWrap(true);
    _ui->AvailableModelsLabel->setWordWrap(true);

    _ui->InvalidUrlLabel->hide();
    
    _ui->ResetToDefaultUrlButton->setDisabled(true);
    _ui->StartSessionButton->setDisabled(true);
    _ui->RefreshButton->setDisabled(true);

    //  [ ! ] TODO: not llow newline character in text edit and give invalid url error
    auto cursor = _ui->ChangeUrl->textCursor();
    cursor.insertText(Api::Endpoints::get_endpoints()->get_base_url().toString());
    _ui->ChangeUrl->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _started_editing = false;

    connect(_ui->ConnectButton, &QPushButton::clicked, this, [this](){
        _ui->StartSessionButton->setDisabled(true);
        _ui->RefreshButton->setDisabled(true);
        if (!Api::Endpoints::get_endpoints()->set_base_url(_ui->ChangeUrl->toPlainText())) {
            _ui->InvalidUrlLabel->setText("Invalid URL" + _ui->ChangeUrl->toPlainText());
            _ui->InvalidUrlLabel->show();
            _ui->ChangeUrl->clear();
            auto cursor = _ui->ChangeUrl->textCursor();
            cursor.insertText(Api::Endpoints::get_endpoints()->get_base_url().toString());
        }
        else {
            _ui->ConnectButton->setDisabled(true);
            _ui->InvalidUrlLabel->hide();
            try_fetch_tags();
        }
    });

    connect(_ui->RefreshButton, &QPushButton::clicked, this, [this](){
        _ui->StartSessionButton->setDisabled(true);
        _ui->RefreshButton->setDisabled(true);
        try_fetch_tags();
    });

    connect(_ui->ChangeUrl, &QPlainTextEdit::textChanged, this, [this](){
        if (!_started_editing) {
            _started_editing = true;
            _ui->StartSessionButton->setDisabled(true);
            _ui->RefreshButton->setDisabled(true);
            _ui->ConnectButton->setDisabled(false);
            _ui->ResetToDefaultUrlButton->setDisabled(false);
            _ui->ChangeUrl->setFixedHeight(_ui->ChangeUrl->height());
        }
    });

    connect(_ui->ResetToDefaultUrlButton, &QPushButton::clicked, this, [this, &cursor](){
        _ui->StartSessionButton->setDisabled(true);
        _ui->RefreshButton->setDisabled(true);
        _ui->InvalidUrlLabel->show();
        Api::Endpoints::get_endpoints()->reset_base_url_to_default();
        _ui->ChangeUrl->clear();
        auto cursor = _ui->ChangeUrl->textCursor();
        cursor.insertText(Api::Endpoints::get_endpoints()->get_base_url().toString());
        _ui->ResetToDefaultUrlButton->setDisabled(true);
    });

    connect(_ui->StartSessionButton, &QPushButton::clicked, this, [this](){
        emit server_was_selected_signal();
    });
}

SelectServer::~SelectServer() {
    delete _ui;
}


// ----------------------------------------------------------------------------------
// Private member functions
// ----------------------------------------------------------------------------------

void SelectServer::try_fetch_tags() {

    _ui->AvailableModelsLabel->setText("Connecting...");
    _ui->modelPreview->clear();

    qDebug() << "fetching tags";
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
                    _ui->AvailableModelsLabel->setText("Models available on " + Api::Endpoints::get_endpoints()->get_base_url().toString() + ":");
                    for (int i = 0; i < models.size(); i++) {
                        QJsonObject model = models[i].toObject();
                        _ui->modelPreview->addItem(model.value("name").toString());
                    }
                    _ui->StartSessionButton->setDisabled(false);
                }
                else if (json_doc.isArray()) 
                {
                    _ui->AvailableModelsLabel->setText("Something went wrong");
                    qDebug() << "SelectServer::try_fetch_tags - Error: tags json doc is array";
                }
                else {
                    _ui->AvailableModelsLabel->setText("Something went wrong");
                    qDebug() << "SelectServer::try_fetch_tags - Error: tags json doc neither array nor object";
                }
            }
        }
        else {
            _ui->AvailableModelsLabel->setText("No connection on " + Api::Endpoints::get_endpoints()->get_base_url().toString());
            qDebug() << "SelectServer::try_fetch_tags - Error: Ollama server not responding. Tags could not be fetched";
        }
        reply->deleteLater();
        _ui->RefreshButton->setDisabled(false);
    });
}
