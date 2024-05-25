// #include <ui/RequestManager.h>

// // namespace Ui {

// RequestManager::RequestManager()
//     : _network_manager(new QNetworkAccessManager())
// {
//     _api_base_url = QUrl("http://127.0.0.1:11434/");
// }

// RequestManager::~RequestManager() {
// }


// void RequestManager::model_names_to_list(MainWindow *ui) {

//     QNetworkRequest request;
//     request.setUrl(_api_base_url.resolved(QUrl("api/tags")));

//     QNetworkReply *reply = _network_manager->get(request);

//     QObject::connect(reply, &QNetworkReply::finished, this, [reply, ui]() {
//         QRestReply restReply(reply);
//         if (restReply.isSuccess()) {
//             QByteArray response = reply->readAll();
//                 if (!response.isEmpty()) {
//                     QJsonDocument json_doc =  QJsonDocument::fromJson(response);
//                     if (json_doc.isObject()) {
//                         QJsonObject json_obj = json_doc.object();
//                         qDebug() << "Error: json doc is object;";
//                         QJsonArray models = json_obj.value("models").toArray();
//                         for (int i; i < models.size(); i++) {
//                             QJsonObject model = models[i].toObject();
//                             ui->ModelListWidget->addItem(model.value("name").toString());
//                         }
//                     }
//                     if (json_doc.isArray()) {
//                         qDebug() << "Error: json doc is array";
//                     }
//                     else
//                         qDebug() << "Error: json doc neither array nor object";
//                 }
//         }
//     });
// }



// // } // end namespace Ui
