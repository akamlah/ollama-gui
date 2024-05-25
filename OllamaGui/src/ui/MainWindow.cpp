#include <ui/MainWindow.h>
#include <ui/SelectModel.h>
#include <ui/Chat.h>
// #include <Api.h>

MainWindow::MainWindow(QMainWindow *parent) 
    : QMainWindow(parent)
    , _stackedWidget(new QStackedWidget)
    // , _network_manager(new QNetworkAccessManager())
{
    // Api::Endpoints* endpoints = Api::Endpoints::get_endpoints();
    // QUrl base_url = endpoints->get_base_url();
    // qDebug() << base_url;
    // qDebug() << endpoints->api_urls_get.tags_url;
    // QProcess ollama_run;
    // ollama_run.start("ollama");
    setCentralWidget(_stackedWidget); 

    SelectModel *select_model = new SelectModel(this);
    _stackedWidget->addWidget(select_model);

    QObject::connect(
        select_model, 
        SIGNAL(model_was_selected_signal(QString)),
        this,
        SLOT(model_was_selected_slot(QString))
    );

}

void MainWindow::model_was_selected_slot(QString name) {
    // qDebug() << "Running: " << name;
    Chat *chat = new Chat(name, this);
    _stackedWidget->addWidget(chat);
    _stackedWidget->setCurrentIndex(_stackedWidget->count() - 1);
    QObject::connect(
        chat,
        SIGNAL(change_model_request_signal()),
        this,
        SLOT(change_model_request_slot())
    );
}

void MainWindow::change_model_request_slot() {
    // qDebug() << "Running: " << name;
    _stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow() {
    // delete chat & model pointers
    delete _stackedWidget;
}


// } // end namespace Ui
