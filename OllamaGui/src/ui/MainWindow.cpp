#include <ui/MainWindow.h>
#include <ui/SelectModel.h>
#include <ui/Chat.h>
#include <ui/SelectServer.h>
#include <api/Endpoints.h>

// ----------------------------------------------------------------------------------
//
//  MainWindow: main child widgets
// 
//
// 
//         |----- QHBoxLayout = {url info, settings button, "nav" button}
//         |                                                                  
//   ______|____________________________________
//  |   ___|______________________________      |
//  |  | --|----------------------------- |   +-----central widget
//  |  | | +    navigation area         | |     |
//  |  | -------------------------------- |     |
//  |  |                                  |     |
//  |  | -------------------------------- |     |+-- MainWindow
//  |  | |   stacked widget             | |     |
//  |  | |                              | |     |
//  |  | |                           +---------------Stacked Widget = {
//  |  | -------------------------------- |     |        SelectServer view (index 0) only on bootup,
//  |  |__________________________________|     |        SelectModel view (index 1),    <---|  
//  |___________________________________________|        Chat Tab Widget view (index 3) <---|  switching
//                                                   }
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

    source_stylesheet();
    // setMinimumSize(960, 540);
    auto screen = this->screen();
    setMinimumSize(screen->size() / 2);

    setCentralWidget(_central_widget);

    // configure central Widget layout
    QGridLayout *central_widget_layout = new QGridLayout(_central_widget);
    _central_widget->setLayout(central_widget_layout);

    // configure navigation area (including actions)
    configure_header_layout();
    central_widget_layout->addItem(_header_layout, 0, 0);

    // add stacked widget containing all app views to central widget
    central_widget_layout->addWidget(_stackedWidget, 1, 0);

    // configure stacked vidget's model selection view and set that to current index
    configure_stacked_widget();

    // setup shortcuts for zoom
    setup_zoomin_zoomout_shortcuts();
}


MainWindow::~MainWindow() {
    // as parent goes out of scope, all children widgets
    // are deleted in qt -> no need for manual deallocation
    // of all QWidget instances
}


// ----------------------------------------------------------------------------------
// Private member functions for initial setup
// ----------------------------------------------------------------------------------


/// @brief Tries to fetch stylesheet from resources.qrc (as in build system) and
/// apply it to the application. Prints debug message on error.
void MainWindow::source_stylesheet() {
    // path defined as realtive to qt resource file as defined in CMakeLists
    QString stylesheet_rescource_path = ":/styles/main_stylesheet.qss";
    QFile file(stylesheet_rescource_path);
    if (!file.open(QFile::ReadOnly))
        qDebug() << "MainWindow::source_stylesheet - Error: could not open file " 
            << stylesheet_rescource_path;
    else {
        QString style_sheet_as_string = QLatin1String(file.readAll());
        qApp->setStyleSheet(style_sheet_as_string);
    }
}


/// @brief Created a new layout containing all the elements in the navigation
/// area in the main window and necessary connections for signals. 
/// @return header layout
void MainWindow::configure_header_layout() {

    _header_layout = new QHBoxLayout();

    // Url Label
    _url_label = new QLabel(_central_widget);
    _url_label->setObjectName("UrlInstancelabel");
    _url_label->setWordWrap(true);
    _header_layout->addWidget(_url_label, 0);
    _url_label->hide();
    
    // Navigation Button
    _nav_button = new QPushButton(_central_widget);
    _nav_button->setObjectName("NavButton");
    _header_layout->addWidget(_nav_button, 2, Qt::AlignRight);
    _nav_button->hide();
    connect(_nav_button, &QPushButton::clicked, this, [this](){
        if (_stackedWidget->currentIndex() == StackedViews::SelectModelView) {
            _stackedWidget->setCurrentIndex(StackedViews::ChatView);
            _nav_button->setText("New Model");
        }
        else {
            _stackedWidget->setCurrentIndex(StackedViews::SelectModelView);
            _nav_button->setText("Back to open conversations");
        }
    });

    // Configure Layout stretch
    _header_layout->setStretch(0, 20);
    _header_layout->setStretch(1, 1);
}


/// @brief Instantiates the SelectServer view, Model Selection View and Tab Widget
/// that will contain the chats, sets the initial view to Server Select, and
/// connects all signals and slots, essantialy the core logic of all the main component's
/// interactions.
void MainWindow::configure_stacked_widget() {

    // settings view
    SelectServer * server_select_widget = new SelectServer(_central_widget);
    _stackedWidget->addWidget(server_select_widget);
    QObject::connect(
        server_select_widget, 
        SIGNAL(server_was_selected_signal()),
        this,
        SLOT(server_was_selected_slot())
    );

    // select model view
    SelectModel *select_model = new SelectModel(_central_widget);
    _stackedWidget->addWidget(select_model);
    QObject::connect(
        select_model, 
        SIGNAL(model_was_selected_signal(QString)),
        this,
        SLOT(model_was_selected_slot(QString))
    );

    QObject::connect(
        server_select_widget, 
        SIGNAL(server_was_selected_signal()),
        select_model,
        SLOT(fetch_tags())
    );

    // chat tabs
    _stackedWidget->addWidget(_tabWidget);

    // set select model to current view
    _stackedWidget->setCurrentIndex(StackedViews::SelectServerView);
}

/// @brief Confugures Ctrl/Cmd + '+'/'-' as zoom in/out shortcuts for the entire ui
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


// ----------------------------------------------------------------------------------
// Slots
// ----------------------------------------------------------------------------------

void MainWindow::server_was_selected_slot() {
    _stackedWidget->setCurrentIndex(StackedViews::SelectModelView);
    _url_label->show();
    _url_label->setText( "Currentnly connected to ollama server instance at: " +
        Api::Endpoints::get_endpoints()->get_base_url().toString() );
}


/// @brief If model already opened, send back to that tab, elsecreate a new chat panel,
/// connect the "Disconnect" button from the chat to the main window and add it as
/// a tab. Then page back to chat view
/// @param name of the model
void MainWindow::model_was_selected_slot(QString name) {
    _nav_button->setText("New Model");
    for (int i = 0; i < _tabWidget->count(); i++) {
        if (_tabWidget->tabText(i) == name) {
            _tabWidget->setCurrentIndex(i);
            _stackedWidget->setCurrentIndex(StackedViews::ChatView);
            return ;
        }
    }
    Chat *chat = new Chat(name, _tabWidget);
    _nav_button->show();
    _tabWidget->addTab(chat, name);
    _tabWidget->setCurrentIndex(_tabWidget->count() - 1);
    _stackedWidget->setCurrentIndex(StackedViews::ChatView);
    QObject::connect(
        chat,
        SIGNAL(close_conversation_request_signal()),
        this,
        SLOT(close_conversation_request_slot())
    );
}

/// @brief call chat object destructor and remove tab from tabstack
/// Change to Model selection view if no tabs remain open
void MainWindow::close_conversation_request_slot() {
    _tabWidget->currentWidget()->deleteLater();
    _tabWidget->removeTab(_tabWidget->currentIndex());
    if (_tabWidget->count() == 0) {
        _stackedWidget->setCurrentIndex(StackedViews::SelectModelView);
    }
}


// } // end namespace Ui
