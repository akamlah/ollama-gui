#ifndef ENDPOINTS_H
#define ENDPOINTS_H

#include <QUrl>
#include <QObject>

namespace Api {
    // class Endpoints;
    // class Request;
// }
/* 
Content:
    class Endpoints;
    class Request;
*/

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
class Endpoints: public QObject {

    private:

        // api base url
        static QUrl ollama_base_url;

        // endpoints are defined here
        struct ollama_get_urls {
            QUrl tags_url = ollama_base_url.resolved(QUrl("api/tags"));
        };
        struct ollama_post_urls {
            QUrl generate_url = ollama_base_url.resolved(QUrl("api/generate"));
            QUrl chat_url = ollama_base_url.resolved(QUrl("api/chat"));
        };

    public:
        
        typedef struct ollama_get_urls t_ollama_get_urls;
        typedef struct ollama_post_urls t_ollama_post_urls;
        static const t_ollama_get_urls api_urls_get;
        static const t_ollama_post_urls api_urls_post;

    protected:

        static Endpoints* instance;

    protected:

        Endpoints() { }

    public:

        // inhibit copy
        Endpoints(Endpoints& other) = delete;
        Endpoints& operator=(Endpoints& other) = delete;

        // get singleton instance
        static Endpoints* get_endpoints();

        // get and set base url
        const QUrl& get_base_url();
        void set_base_url(const QUrl& url);

};


/* 
* Class that stores pre-formatted json objects for post requests
*/
class Request {
    public:
    
};


}


#endif // ENDPOINTS_H