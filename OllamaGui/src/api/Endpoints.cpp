#include <api/Endpoints.h>

namespace Api{

QUrl Endpoints::ollama_base_url = QUrl("http://127.0.0.1:11434");
const Endpoints::t_ollama_post_urls Endpoints::api_urls_post;
const Endpoints::t_ollama_get_urls Endpoints::api_urls_get;
Endpoints* Endpoints::instance = nullptr;

// Protected constructor
Endpoints::Endpoints() {
}

// get singleton instance
static Endpoints* get_endpoints();

// get and set base url
const QUrl& Endpoints::get_base_url() {
    return ollama_base_url;
}

void Endpoints::set_base_url(const QUrl& url) {
    ollama_base_url = url;    
}

Endpoints* Endpoints::get_endpoints() {
    if (Endpoints::instance == nullptr)
        Endpoints::instance = new Endpoints;
    return Endpoints::instance; 
}

} // NAMESPACE Api