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
    Chat *chat = new Chat(this);
    _stackedWidget->addWidget(select_model);
    _stackedWidget->addWidget(chat);

    QObject::connect(
        select_model, 
        &SelectModel::model_was_selected_signal, 
        this,
        &MainWindow::model_was_selected_slot
    );
}

void MainWindow::model_was_selected_slot(QString name) {
    _stackedWidget->setCurrentIndex(1);
    qDebug() << name;
}

MainWindow::~MainWindow() {
    // delete chat & model pointers
    delete _stackedWidget;
}


// } // end namespace Ui
