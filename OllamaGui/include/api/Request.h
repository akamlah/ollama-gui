#ifndef REQUEST_H
#define REQUEST_H

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <vector>

namespace Api {

class Request: public QNetworkRequest {

    public:

        explicit Request();
        virtual ~Request();

        // inhibit copy
        Request(Request& other) = delete;
        Request& operator=(Request& other) = delete;

        std::vector<QString> get_model_list();

};


}


#endif // REQUEST_H