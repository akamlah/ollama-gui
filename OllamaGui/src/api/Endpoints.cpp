#include <api/Endpoints.h>

namespace Api{

const QUrl Endpoints::ollama_default_base_url = QUrl("http://127.0.0.1:11434");
QUrl Endpoints::ollama_current_base_url = Endpoints::ollama_default_base_url;
Endpoints::t_ollama_post_urls Endpoints::api_urls_post;
Endpoints::t_ollama_get_urls Endpoints::api_urls_get;
Endpoints* Endpoints::instance = nullptr;

// Protected constructor
Endpoints::Endpoints() {
}

// get singleton instance
static Endpoints* get_endpoints();

// get and set base url
const QUrl& Endpoints::get_base_url() {
    return ollama_current_base_url;
}

const QUrl& Endpoints::get_default_base_url() {
    return ollama_default_base_url;
}

int Endpoints::set_base_url(const QString& url) {
    if (QUrl(url).isValid()) {
        ollama_current_base_url = QUrl(url);
        api_urls_get.refresh();
        api_urls_post.refresh();
        return (1);
    }
    else {
        return (0);
    }
}

void Endpoints::reset_base_url_to_default() {
    ollama_current_base_url = ollama_default_base_url;
    api_urls_get.refresh();
    api_urls_post.refresh();
}

Endpoints* Endpoints::get_endpoints() {
    if (Endpoints::instance == nullptr)
        Endpoints::instance = new Endpoints;
    return Endpoints::instance; 
}

} // NAMESPACE Api