#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include <QUrl>
#include <QObject>
#include <QString>

namespace Api {

/*!
 * \class Endpoints
 * \brief Singleton to store and access the ollama api endpoints and base url
 * 
 * \code{.cpp}
 *  Example for access:
 * 
 *  #include <Api.h>
 * 
 *  QUrl url_tags = Api::Endpoints::get_endpoints()->api_urls_get.tags_url
 *  returns something like "QUrl("http://127.0.0.1:3000/api/tags")"
 *
 *  Api::Endpoints::get_endpoints()->get_base_url()
 *  returns "QUrl("http://127.0.0.1:3000")"
 * \endcode
 * 
 */
class Endpoints {

    private:

        // api base url
        static const QUrl ollama_default_base_url;
        static QUrl ollama_current_base_url;

        // endpoints are defined here (change to less hardcoded solution [ ! ])
        struct ollama_get_urls {
            QUrl tags_url;
            ollama_get_urls() {
                refresh();
            }
            void refresh() {
                tags_url = ollama_current_base_url.resolved(QUrl("api/tags"));
            }
        };
        struct ollama_post_urls {
            QUrl generate_url;
            QUrl chat_url;
            ollama_post_urls() {
                refresh();
            }
            void refresh() {
                generate_url = ollama_current_base_url.resolved(QUrl("api/generate"));
                chat_url = ollama_current_base_url.resolved(QUrl("api/chat"));
            }
        };

    public:
        
        typedef struct ollama_get_urls t_ollama_get_urls;
        typedef struct ollama_post_urls t_ollama_post_urls;

        static t_ollama_get_urls api_urls_get;
        static t_ollama_post_urls api_urls_post;

    protected:

        static Endpoints* instance;

    protected:

        Endpoints();

    public:

        // inhibit copy
        Endpoints(Endpoints& other) = delete;
        Endpoints& operator=(Endpoints& other) = delete;

        // get singleton instance
        static Endpoints* get_endpoints();

        // get and set base url
        const QUrl& get_default_base_url();
        const QUrl& get_base_url();
        void reset_base_url_to_default();
        int set_base_url(const QString& url);

};


}


#endif // ENDPOINTS_H