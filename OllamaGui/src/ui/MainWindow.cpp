#include <ui/MainWindow.h>
#include <ui/SelectModel.h>
#include <ui/Chat.h>
// #include <Api.h>

MainWindow::MainWindow(QMainWindow *parent) 
    : QMainWindow(parent)
    // , _stackedWidget(new QStackedWidget(this))
    // , _tabWidget(new QTabWidget(this))
{
    // QProcess ollama_run;
    // ollama_run.start("ollama");
    QFile file("/home/dom/alice/qt/ollama_gui/OllamaGui/src/ui/stylesheet.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);

    this->setMinimumSize(960, 540);

    QWidget *central_widget = new QWidget(this);
    QGridLayout *layout = new QGridLayout(central_widget);
    setCentralWidget(central_widget);
    central_widget->setLayout(layout);

    _stackedWidget = new QStackedWidget(central_widget);
    _tabWidget = new QTabWidget(central_widget);

    _nav_button = new QPushButton(central_widget);
    // _nav_button->setFixedSize(_nav_button->size());
    // _nav_button->setMinimumWidth(Qt::MinimumSize);

    layout->addWidget(_nav_button, 0, 0, Qt::AlignRight);
    _nav_button->hide();
    connect(_nav_button, &QPushButton::clicked, this, [this](){
        if (_stackedWidget->currentIndex() == 0) {
            _nav_button->setText("New Model");
            _stackedWidget->setCurrentIndex(1); 
        }
        else {
            _nav_button->setText("Back to open conversations");
            _stackedWidget->setCurrentIndex(0); 
        }
    });

    SelectModel *select_model = new SelectModel(central_widget);
    _stackedWidget->addWidget(select_model);
    _stackedWidget->addWidget(_tabWidget);
    _stackedWidget->setCurrentIndex(0);

    layout->addWidget(_stackedWidget, 1, 0);

    QObject::connect(
        select_model, 
        SIGNAL(model_was_selected_signal(QString)),
        this,
        SLOT(model_was_selected_slot(QString))
    );
}

void MainWindow::model_was_selected_slot(QString name) {
    // qDebug() << "Running: " << name;

    for (int i = 0; i < _tabWidget->count(); i++) {
        if (_tabWidget->tabText(i) == name) {
            _stackedWidget->setCurrentIndex(1);
            _nav_button->setText("New Model");
            return ;
        }
    }

    Chat *chat = new Chat(name, _tabWidget);
    _tabWidget->addTab(chat, name);
    // _stackedWidget->addWidget(chat);
    // _stackedWidget->setCurrentIndex(_stackedWidget->count() - 1);
    _nav_button->show();
    _nav_button->setText("New Model");
    _stackedWidget->setCurrentIndex(1);
    QObject::connect(
        chat,
        SIGNAL(close_conversation_request_signal()),
        this,
        SLOT(close_conversation_request_slot())
    );

}

void MainWindow::close_conversation_request_slot() {
    _tabWidget->currentWidget()->deleteLater();  // calls chat destructor
    _tabWidget->removeTab(_tabWidget->currentIndex());
}

MainWindow::~MainWindow() {
    // delete chat & model pointers
    delete _stackedWidget;
}


// } // end namespace Ui
