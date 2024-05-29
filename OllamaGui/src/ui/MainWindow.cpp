#include <ui/MainWindow.h>
#include <ui/SelectModel.h>
#include <ui/Chat.h>
#include <api/Api.h>

// ----------------------------------------------------------------------------------
//
//  MainWindow: main child widgets
// 
//
// 
//         |----- QHBoxLayout = {url info, settings button, "nav" button}
//         |                                                                  
//  _______|____________________________________
//  |  ____|_______________________________     |
//  |  | --|----------------------------- |   +-----central widget
//  |  | | +    navigation area         | |     |
//  |  | -------------------------------- |     |
//  |  |                                  |     |
//  |  | -------------------------------- |     |+-- MainWindow
//  |  | |   stacked widget             | |     |
//  |  | |                              | |     |
//  |  | |                           +---------------Stacked Widget = {
//  |  | -------------------------------- |     |        SelectModel view (index 0),
//  |  |__________________________________|     |        Chat Tab Widget (index 1)
//  |___________________________________________|    }
//
//
//                                       ( Tab Widget = Chat instances )
//
// ----------------------------------------------------------------------------------



// ----------------------------------------------------------------------------------
// Constructor, destructor
// ----------------------------------------------------------------------------------

MainWindow::MainWindow(QMainWindow *parent) 
    : QMainWindow(parent)
    , _central_widget(new QWidget(this))
    , _stackedWidget(new QStackedWidget(_central_widget))
    , _tabWidget(new QTabWidget(_central_widget))
{
    // [ ! ] Handle connection test and server not running case
    // QProcess ollama_run;
    // ollama_run.start("ollama");

    this->setMinimumSize(960, 540);
    this->setCentralWidget(_central_widget);

    QGridLayout *central_widget_layout = new QGridLayout(_central_widget);
    _central_widget->setLayout(central_widget_layout);

    QBoxLayout * header_layout = this->setup_central_widget_navigation_area();
    central_widget_layout->addLayout(header_layout, 0, 0);

    this->create_new_model_selection_view();
    central_widget_layout->addWidget(_stackedWidget, 2, 0);

    this->setup_zoomin_zoomout_shortcuts();
}

MainWindow::~MainWindow() {
    delete _stackedWidget;
}


// ----------------------------------------------------------------------------------
// Slots
// ----------------------------------------------------------------------------------

/// @brief If model already opened, send back to that tab, elsecreate a new chat panel,
/// connect the "Disconnect" button from the chat to the main window and add it as
/// a tab. Then page back to chat view
/// @param name of the model
void MainWindow::model_was_selected_slot(QString name) {
    for (int i = 0; i < _tabWidget->count(); i++) {
        if (_tabWidget->tabText(i) == name) {
            _tabWidget->setCurrentIndex(i);
            _stackedWidget->setCurrentIndex(1);
            _nav_button->setText("New Model");
            return ;
        }
    }
    Chat *chat = new Chat(name, _tabWidget);
    _tabWidget->addTab(chat, name);
    _tabWidget->setCurrentIndex(_tabWidget->count() - 1);
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


/// @brief call chat object destructor and remove tab from tabstack
void MainWindow::close_conversation_request_slot() {
    _tabWidget->currentWidget()->deleteLater();
    _tabWidget->removeTab(_tabWidget->currentIndex());
}


// ----------------------------------------------------------------------------------
// Private member functions for initial setup
// ----------------------------------------------------------------------------------


/// @brief Tries to fetch stylesheet from resources.qrc (as in build system) and
/// apply it to the application. Prints debug message on error.
void MainWindow::source_stylesheet() {
    QString stylesheet_rescource_path = ":/stylesheet.qss";
    QFile file(stylesheet_rescource_path);
    if (!file.open(QFile::ReadOnly))
        qDebug() << "Error: could not open file " << stylesheet_rescource_path;
    QString style_sheet_as_string = QLatin1String(file.readAll());
    qApp->setStyleSheet(style_sheet_as_string);
}


/// @brief Created a new layout containing all the elements in the navigation
/// area in the main window and necessary connections for signals. 
/// @return header layout
QBoxLayout* MainWindow::setup_central_widget_navigation_area() {

    QHBoxLayout *header_layout = new QHBoxLayout();

    QLabel *url_label = new QLabel(_central_widget);
    url_label->setObjectName("UrlInstancelabel");
    url_label->setText( "Currentnly connected to ollama server instance at: " +
        Api::Endpoints::get_endpoints()->get_base_url().toString() );
    url_label->setWordWrap(true);
    header_layout->addWidget(url_label, 0);

    QPushButton *settings_btn = new QPushButton(_central_widget);
    settings_btn->setText("Settings");
    header_layout->addWidget(settings_btn, 1, Qt::AlignRight);

    _nav_button = new QPushButton(_central_widget);
    _nav_button->setObjectName("NavButton");
    header_layout->addWidget(_nav_button, 2, Qt::AlignRight);
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

    header_layout->setStretch(0, 20);
    header_layout->setStretch(1, 1);
    header_layout->setStretch(2, 1);
    return header_layout;
}

/// @brief Instantiates the model selection view and adds it to the stack
void MainWindow::create_new_model_selection_view() {

    SelectModel *select_model = new SelectModel(_central_widget);
    _stackedWidget->addWidget(select_model);
    _stackedWidget->addWidget(_tabWidget);
    _stackedWidget->setCurrentIndex(0);

    QObject::connect(
        select_model, 
        SIGNAL(model_was_selected_signal(QString)),
        this,
        SLOT(model_was_selected_slot(QString))
    );
}

void MainWindow::setup_zoomin_zoomout_shortcuts() {
    int default_font_size = this->fontInfo().pixelSize();
    if (default_font_size > 4 && default_font_size < 40) {
        _font_size = default_font_size;
    }
    else {
        _font_size = 13;
    }
    QShortcut *shortcut_plus = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Plus), this);
    connect(shortcut_plus, &QShortcut::activated,
        this, [this](){
            if (_font_size < 40) {
                _font_size ++;
                QString size_str = QString::number(_font_size);
                QWidget::setStyleSheet("font-size:" + size_str + "px;");
            }
    });
    QShortcut *shortcut_minus = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus), this);
    connect(shortcut_minus, &QShortcut::activated,
        this, [this](){
            if (_font_size > 4) {
                _font_size --;
                QString size_str = QString::number(_font_size);
                QWidget::setStyleSheet("font-size:" + size_str + "px;");
            }
    });
}



// } // end namespace Ui
