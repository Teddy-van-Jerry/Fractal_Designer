#include <QLabel>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFile>
#include <QThread>
#include <QSettings>
#include <QCursor>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initTitleBar();

    qDebug() << QThread::currentThreadId();
    QLabel *permanent = new QLabel(this);
    permanent->setText("ALL RIGHTS RESERVED (C) 2021 <strong>TVJ Group</strong> | <strong>Teddy van Jerry</strong>");
    ui->statusbar->addPermanentWidget(permanent);
    ui->statusbar->showMessage(tr("Welcome to Fractal Designer 6.0!"), 20000);
    show_preview_image();
    Project_Template = "Undefined";
    setWindowTitle((Open_Location.isEmpty() ? tr("Unsaved Project") : QFileInfo(Open_Location).fileName()) +
                   " - <strong>Fractal Designer</strong>");

    Line_Search = new QLineEdit(this);
    // Line_Search->setToolTip("Search");
    Line_Search->setFixedSize(180, 20);
    Line_Search->setStyleSheet("border-radius: 10px; border: 1px solid green;");
    Line_Search->setPlaceholderText("Search");
    ui->toolBar->insertWidget(NULL, Line_Search);

    Button_Search = new QToolButton(this);
    Button_Search->setIcon(QIcon(":icon/Menu Icon/Search.svg"));
    Button_Search->setFixedSize(20, 20);
    ui->toolBar->insertWidget(NULL, Button_Search);
    connect(Button_Search, SIGNAL(clicked()), this, SLOT(on_Button_Search_clicked()));
    connect(Line_Search, SIGNAL(returnPressed()), this, SLOT(on_Button_Search_clicked()));

    // QSpacerItem* Spacer_ToolBar = new QSpacerItem(-1, 20);
    QLabel* Label_Spacer = new QLabel(this);
    Label_Spacer->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
    ui->toolBar->insertWidget(NULL, Label_Spacer);

    Button_Login_MainWindow = new QPushButton();
    Button_Login_MainWindow->setText(tr("Log In"));
    Button_Login_MainWindow->setObjectName("Button_Login_MainWindow");
    ui->toolBar->insertWidget(NULL, Button_Login_MainWindow);
    connect(Button_Login_MainWindow, SIGNAL(clicked()), this, SLOT(on_Button_Login_MainWindow_clicked()));

    Label_User_Name = new QLabel(this);
    Label_User_Name->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    Label_User_Name->setStyleSheet("color: green; font: Consolas; font-size: 14px;");
    ui->toolBar->insertWidget(NULL, Label_User_Name);

    table_route_model->setColumnCount(6);
    table_route_model->setHeaderData(0, Qt::Horizontal, "t");
    table_route_model->setHeaderData(1, Qt::Horizontal, "x");
    table_route_model->setHeaderData(2, Qt::Horizontal, "y");
    table_route_model->setHeaderData(3, Qt::Horizontal, "angle");
    table_route_model->setHeaderData(4, Qt::Horizontal, "width");
    table_route_model->setHeaderData(5, Qt::Horizontal, "v rate");
    QStyledItemDelegate* preciseDoubleDelegate = new QStyledItemDelegate(ui->tableView_Route);
    preciseDoubleDelegate->setItemEditorFactory(&m_factory);
    ui->tableView_Route->setItemDelegate(preciseDoubleDelegate);
    ui->tableView_Route->setModel(table_route_model);
    ui->tableView_Route->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_Route->setSortingEnabled(true);
    table_route_model->setSortRole(Qt::AscendingOrder);
    //ui->tableView_Route->setItemDelegate;
    connect(table_route_model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(routeEdit(QStandardItem*)));
    connect(ui->tableView_Route->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(dealRouteSort(int)));

    ui->actionStop->setDisabled(true);

    route_tool_window = new Route_Tool(this);

    editor = new FRD_Editor(ui->gridLayout_Editor);
    connect(editor, SIGNAL(textChanged()), this, SLOT(updateEditorInfo()));

    // Route custom menu
    table_route_menu = new QMenu(ui->tableView_Route);
    table_route_action[0] = new QAction(tr("Move up"), ui->tableView_Route);
    table_route_action[1] = new QAction(tr("Move down"), ui->tableView_Route);
    table_route_action[2] = new QAction(tr("Insert before"), ui->tableView_Route);
    table_route_action[3] = new QAction(tr("Insert after"), ui->tableView_Route);
    table_route_action[4] = new QAction(tr("Delete this row"), ui->tableView_Route);
    connect(table_route_action[0], SIGNAL(triggered()), this, SLOT(tableRouteMoveUp()));
    connect(table_route_action[1], SIGNAL(triggered()), this, SLOT(tableRouteMoveDown()));
    connect(table_route_action[2], SIGNAL(triggered()), this, SLOT(tableRouteInsertBefore()));
    connect(table_route_action[3], SIGNAL(triggered()), this, SLOT(tableRouteInsertAfter()));
    connect(table_route_action[4], SIGNAL(triggered()), this, SLOT(tableRouteDeleteRow()));
    ui->tableView_Route->setContextMenuPolicy(Qt::CustomContextMenu);
    table_route_menu->addActions({table_route_action[0], table_route_action[1]});
    table_route_menu->addSeparator();
    table_route_menu->addActions({table_route_action[2], table_route_action[3]});
    table_route_menu->addSeparator();
    table_route_menu->addActions({table_route_action[4]});
    connect(ui->tableView_Route, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(tableRouteCustomMenuRequested(QPoint)));

    ui->splitter_Editor_Up->setStretchFactor(0, 4);
    ui->splitter_Editor_Up->setStretchFactor(1, 1);
    ui->splitter_Editor_Up->setSizes({10000, 100});
    ui->splitter_Editor->setStretchFactor(0, 4);
    ui->splitter_Editor->setStretchFactor(1, 1);
    ui->splitter_Editor->setSizes({10000, 10});

    error_list_model->setColumnCount(6);
    error_list_model->setHeaderData(0, Qt::Horizontal, "Type");
    error_list_model->setHeaderData(1, Qt::Horizontal, "Error Code");
    error_list_model->setHeaderData(2, Qt::Horizontal, "Row");
    error_list_model->setHeaderData(3, Qt::Horizontal, "Col");
    error_list_model->setHeaderData(4, Qt::Horizontal, "Length");
    error_list_model->setHeaderData(5, Qt::Horizontal, "Details");
    ui->tableView_error->setModel(error_list_model);
    ui->tableView_error->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_error->horizontalHeader()->setMinimumSectionSize(30);
    ui->tableView_error->setSortingEnabled(true);
    ui->tableView_error->setEditTriggers(QAbstractItemView::NoEditTriggers);
    error_list_model->setSortRole(Qt::AscendingOrder);

    ReadStyle();
    updateMaxButton();
}

MainWindow::~MainWindow()
{
    //delete image;
    delete this->m_titleBar;
    delete this->m_leftBorder;
    delete this->m_rightBorder;
    delete this->m_bottomBorder;

    if (this->m_menuBar) delete this->m_menuBar;
    else if (this->m_menuWidget) delete this->m_menuWidget;
    delete ui;
}

void MainWindow::initTitleBar()
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinimizeButtonHint);
    this->m_titleBar = new class::FRD_TitleBar(this);
    connect(this->m_titleBar, &FRD_TitleBar::requestClose,this, &MainWindow::close);
    connect(this->m_titleBar, &FRD_TitleBar::requestMaximize, [this]{if (this->isMaximized()) this->showNormal();else this->showMaximized();});
    connect(this->m_titleBar, &FRD_TitleBar::requestMinimize,this, &MainWindow::showMinimized);
    connect(this, &QMainWindow::windowTitleChanged, this->m_titleBar,&QWidget::setWindowTitle);
    connect(&this->FRD_TitleBar().btn_maximize,SIGNAL(clicked()),this,SLOT(updateMaxButton()));
    connect(&this->FRD_TitleBar(),SIGNAL(doubleClicked()),this,SLOT(updateMaxButton()));

    this->m_titleBarW = new QWidget();
    this->m_titleBarW->setMouseTracking(true);
    this->m_titleBarW->installEventFilter(this);

    this->m_menuBar = ui->menubar;
    this->setMenuBar(m_menuBar);
    this->m_menuWidget = nullptr;

    QPixmap *pixmap = new QPixmap(":/EXE Icons/FRD.ico");
    pixmap->scaled(this->FRD_icon.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    this->FRD_icon.setScaledContents(true);
    this->FRD_icon.setPixmap(*pixmap);
    this->FRD_icon.setMinimumSize(16,16);
    this->FRD_icon.setMaximumSize(16,16);

    QHBoxLayout *titleLayout = new QHBoxLayout();
    titleLayout->setContentsMargins(6, 0, 6, 0);
    titleLayout->setSpacing(0);
    titleLayout->addWidget(&FRD_icon);
    titleLayout->addWidget(this->m_menuBar);
    titleLayout->addWidget(this->m_titleBar);

    this->m_titleBarW->setLayout(titleLayout);
    QMainWindow::setMenuWidget(this->m_titleBarW);

    this->m_leftBorder = generateBorder(Qt::LeftToolBarArea, Qt::Vertical);
    this->m_rightBorder = generateBorder(Qt::RightToolBarArea, Qt::Vertical);
    this->m_bottomBorder = generateBorder(Qt::BottomToolBarArea, Qt::Horizontal);


    this->showMaximized();



}

void MainWindow::setOpenLocation(QString str)
{
    Open_Location = str;
    if(str != "")
    {
        save_or_not = true;
        OpenFRD(OPEN_FILE_OUT);
    }
    else
    {

    }
}

inline void _complex_in(QDataStream& in, std::complex<double>& c)
{
    double r = 0, i = 0;
    in >> r >> i;
    std::complex<double> new_c { r, i };
    c = new_c;
}

bool MainWindow::OpenFRD(int type)
{
    ui->actionClose->setEnabled(true);

    QFile FRD_R(Open_Location);
    FRD_R.open(QIODevice::ReadOnly);
    QTextStream in(&FRD_R);
    QString text = in.readAll();
    editor->setText(text);
    FRD_R.close();
    qDebug() << "FRD File Open succeed!";

    display();
    save_or_not = true;

    setWindowTitle(QFileInfo(Open_Location).fileName() + " - <strong>Fractal Designer</strong>");
    return true;
}

void MainWindow::on_Button_Login_MainWindow_clicked()
{
    if(!User_Name.isEmpty())
    {
        QMessageBox::information(this, "Information", "You have already logged in.");
        return;
    }
    Login* l = new Login(this);
    connect(l, &Login::user_name, this, &MainWindow::getUserName);
    l->show();
    l->exec();
}

void MainWindow::on_Button_Search_clicked()
{
    if(Line_Search->text().isEmpty()) return;
    search_result = new Search_Result(this);
    connect(this, &MainWindow::Search_clicked, search_result, &Search_Result::getSearch);
    emit Search_clicked(Line_Search->text());
    search_result->show();
}

void MainWindow::getUserName(QString name)
{
    User_Name = name;
    Label_User_Name->setText(name);
    delete Button_Login_MainWindow;
}

void MainWindow::on_MainWindow_exit_clicked()
{
    close();
}

void MainWindow::on_actionExit_E_triggered()
{
    close();
}

void MainWindow::on_actionChinese_triggered()
{
    QDesktopServices::openUrl(QUrl(Chinese_Help_Url));
}

void MainWindow::on_MainWindow_AboutTVJ_clicked()
{
    QDesktopServices::openUrl(QUrl("https://teddy-van-jerry.org"));
}

void MainWindow::on_pushButton_Chinese_Help_clicked()
{
    qDebug() << "Button clicked";
    MainWindow::on_actionChinese_triggered();
}

void MainWindow::on_actionNew_N_triggered()
{
    if(ui->actionClose->isEnabled())
    {
        on_actionClose_triggered();
    }
    save_or_not = true;

    Open_Location = QFileDialog::getSaveFileName(this,
                                                 tr("New Project"),
                                                 QDir::homePath() + "/Untitled",
                                                 tr("FRD File (*.frd);; FRD Json File (*.frdjson);; Json File (*.json)"));
    if (Open_Location.isEmpty()) return;
    setWindowTitle(QFileInfo(Open_Location).fileName() + " - <strong>Fractal Designer</strong>");
    // print current
    on_actionSave_S_triggered();
}

void MainWindow::on_MainWindow_Newfile_clicked()
{
    MainWindow::on_actionNew_N_triggered();
}

void MainWindow::on_actionOpen_O_triggered()
{
    QString Old_Open_Location = Open_Location;
    QString New_Open_Location = QFileDialog::getOpenFileName(this,
                                                             tr("Open Project"),
                                                             QDir::currentPath(),
                                                             tr("FRD File (*.frd)"));
    qDebug() << New_Open_Location;
    QFile check(New_Open_Location);
    if (check.exists())
    {
        if (ui->actionClose->isEnabled())
        {
            on_actionClose_triggered();
        }
        Open_Location = New_Open_Location;
        if (!OpenFRD(OPEN_FILE_IN))
        {
            Open_Location = Old_Open_Location;
        }
    }
    else if (!New_Open_Location.isEmpty())
    {
        QMessageBox::warning(this, "Opening Project Error", "The file doesn't exist!");
    }
}

void MainWindow::on_MainWindow_openfile_clicked()
{
    MainWindow::on_actionOpen_O_triggered();
}

void MainWindow::show_preview_image()
{
    ui->Label_Preview_Image->setAlignment(Qt::AlignCenter);
    ui->label_previewInVideo->setAlignment(Qt::AlignCenter);

    ui->Label_Preview_Image->resize(ui->Preview_Image->size());
    int preview_w_width(ui->Preview_Image->width());
    int Preview_w_height(ui->Preview_Image->height());
    double rate_w = static_cast<double>(Preview_w_height) / preview_w_width;
    double rate_i = static_cast<double>(image_preview.height()) / image_preview.width();
    if(rate_i > rate_w)
    {
        ui->Label_Preview_Image->setPixmap(QPixmap::fromImage(image_preview).scaledToHeight(Preview_w_height));
    }
    else
    {
        ui->Label_Preview_Image->setPixmap(QPixmap::fromImage(image_preview).scaledToWidth(preview_w_width));
    }

    ui->label_previewInVideo->resize(ui->widget_previewInVideo->size());
    int preview_w_width_video(ui->widget_previewInVideo->width());
    int Preview_w_height_video(ui->widget_previewInVideo->height());
    double rate_w_video = static_cast<double>(Preview_w_height_video) / preview_w_width_video;
    if(rate_i > rate_w_video)
    {
        ui->label_previewInVideo->setPixmap(QPixmap::fromImage(image_preview).scaledToHeight(Preview_w_height_video));
    }
    else
    {
        ui->label_previewInVideo->setPixmap(QPixmap::fromImage(image_preview).scaledToWidth(preview_w_width_video));
    }
}

void MainWindow::on_actionRoute_Tool_triggered()
{
    route_tool_window->show();
}

void MainWindow::resizeEvent(QResizeEvent *Event)
{
    QWidget::resizeEvent(Event);
    ui->Label_Preview_Image->resize(ui->Preview_Image->size());
    int Preview_w_width(ui->Preview_Image->width());
    int Preview_w_height(ui->Preview_Image->height());
    double rate_w = static_cast<double>(Preview_w_height) / Preview_w_width;
    double rate_i = static_cast<double>(image_preview.height()) / image_preview.width();
    if(rate_i > rate_w)
    {
        ui->Label_Preview_Image->setPixmap(QPixmap::fromImage(image_preview).scaledToHeight(Preview_w_height));
    }
    else
    {
        ui->Label_Preview_Image->setPixmap(QPixmap::fromImage(image_preview).scaledToWidth(Preview_w_width));
    }

    ui->label_previewInVideo->resize(ui->widget_previewInVideo->size());
    int preview_w_width_video(ui->widget_previewInVideo->width());
    int Preview_w_height_video(ui->widget_previewInVideo->height());
    double rate_w_video = static_cast<double>(Preview_w_height_video) / preview_w_width_video;
    if(rate_i > rate_w_video)
    {
        ui->label_previewInVideo->setPixmap(QPixmap::fromImage(image_preview).scaledToHeight(Preview_w_height_video));
    }
    else
    {
        ui->label_previewInVideo->setPixmap(QPixmap::fromImage(image_preview).scaledToWidth(preview_w_width_video));
    }
}

void MainWindow::on_Tab_currentChanged(int index)
{
    if(index == 2) // Image
    {
        ui->Label_Preview_Image->resize(ui->Preview_Image->size());
        int preview_w_width(ui->Preview_Image->width());
        int Preview_w_height(ui->Preview_Image->height());
        double rate_w = static_cast<double>(Preview_w_height) / preview_w_width;
        double rate_i = static_cast<double>(image_preview.height()) / image_preview.width();
        if(rate_i > rate_w)
        {
            ui->Label_Preview_Image->setPixmap(QPixmap::fromImage(image_preview).scaledToHeight(Preview_w_height));
        }
        else
        {
            ui->Label_Preview_Image->setPixmap(QPixmap::fromImage(image_preview).scaledToWidth(preview_w_width));
        }
    }

    if(index == 4) // Template
    {
        double rate_i = static_cast<double>(image_preview.height()) / image_preview.width();
        ui->label_previewInVideo->resize(ui->widget_previewInVideo->size());
        int preview_w_width_video(ui->widget_previewInVideo->width());
        int Preview_w_height_video(ui->widget_previewInVideo->height());
        double rate_w_video = static_cast<double>(Preview_w_height_video) / preview_w_width_video;
        if(rate_i > rate_w_video)
        {
            ui->label_previewInVideo->setPixmap(QPixmap::fromImage(image_preview).scaledToHeight(Preview_w_height_video));
        }
        else
        {
            ui->label_previewInVideo->setPixmap(QPixmap::fromImage(image_preview).scaledToWidth(preview_w_width_video));
        }
    }
}

void MainWindow::on_actionSave_S_triggered()
{
    save_or_not = true;
    if(Open_Location == "") on_actionNew_N_triggered();


//    // == Print into file ==
//    curr_info.setColourInfo(ui->Convergent_Points_Colour_Formula->toPlainText(), true);
//    curr_info.setColourInfo(ui->Divergent_Points_Colour_Formula->toPlainText(), false);
//    curr_info.print(Open_Location, FRD_Version);

    // print to frd file
    info.print(Open_Location);
}

void MainWindow::on_Template_Choice_1_toggled(bool checked)
{
    if(checked)
    {
        Project_Template = "1";
        ui->label_Minimum_Unclassified_Value->setText("Minimum unclassified value");
        ui->Min_class_value->setDecimals(6);
        ui->Min_class_value->setToolTip("");
        ui->label_Minimum_Unclassified_Value->setToolTip("");
        ui->lineEdit_Custom_Formula->setDisabled(true);
    }
}

void MainWindow::on_Template_Choice_2_toggled(bool checked)
{
    if(checked)
    {
        Project_Template = "2";
        ui->label_Minimum_Unclassified_Value->setText("Minimum unclassified value");
        ui->Min_class_value->setDecimals(6);
        ui->Min_class_value->setToolTip("");
        ui->label_Minimum_Unclassified_Value->setToolTip("");
        ui->lineEdit_Custom_Formula->setDisabled(true);
        on_actionTemplate_2_triggered();
    }
    else
    {
        template_2_dialog->close();
    }
}

void MainWindow::on_Template_Choice_3_toggled(bool checked)
{
    if(checked)
    {
        Project_Template = "3";
        ui->label_Minimum_Unclassified_Value->setText("Minimum unclassified value");
        ui->Min_class_value->setDecimals(6);
        ui->Min_class_value->setToolTip("");
        ui->label_Minimum_Unclassified_Value->setToolTip("");
        ui->lineEdit_Custom_Formula->setDisabled(true);
    } 
}

void MainWindow::on_Template_Choice_4_toggled(bool checked)
{
    if(checked)
    {
        Project_Template = "4";
        ui->label_Minimum_Unclassified_Value->setText("Solution accuracy");
        ui->Min_class_value->setDecimals(10);
        ui->Min_class_value->setToolTip("The relative accuracy, i.e. the ratio of modulus of difference in two iterations to its modulus.");
        ui->label_Minimum_Unclassified_Value->setToolTip("The relative accuracy, i.e. the ratio of modulus of difference in two iterations to its modulus.");
        ui->lineEdit_Custom_Formula->setDisabled(true);
        on_actionTemplate_6_triggered(); // Choice of Template 4
    }
    else
    {
        template_4_dialog->close();
    }
}

void MainWindow::on_Template_Choice_5_toggled(bool checked)
{
    if(checked)
    {
        Project_Template = "5";
        ui->label_Minimum_Unclassified_Value->setText("Minimum unclassified value");
        ui->Min_class_value->setDecimals(6);
        ui->Min_class_value->setToolTip("");
        ui->label_Minimum_Unclassified_Value->setToolTip("");
        ui->lineEdit_Custom_Formula->setDisabled(false);
    }
}

inline std::complex<double> MainWindow::_curr_complex(const std::complex<double>& c1, const std::complex<double>& c2, double t, double k)
{
    return c1 + (c2 - c1) * ((1 - k) * t + k * t * t);
}

void Read_RGBA(const QString& str1, const QString& str2, std::string Colour1_[4], std::string Colour2_[4])
{
    QStringList Colour1_l = str1.split(';');
    QStringList Colour2_l = str2.split(';');

    qDebug() << "Check RGBA" << Colour1_l << Colour2_l;

    for (const auto& str : Colour1_l)
    {
        QString s;
        for (const auto& ch : str)
        {
            if (!ch.isSpace())
            {
                s.append(ch);
            }
        }
        qDebug() << s;
        if (s.left(2) == "R=")
        {
            Colour1_[0] = s.remove(0, 2).toStdString();
        }
        else if (s.left(2) == "G=")
        {
            Colour1_[1] = s.remove(0, 2).toStdString();
        }
        else if (s.left(2) == "B=")
        {
            Colour1_[2] = s.remove(0, 2).toStdString();
        }
        else if (s.left(2) == "A=")
        {
            Colour1_[3] = s.remove(0, 2).toStdString();
        }
        else
        {
            // just omit that
        }
    }

    for (const auto& str : Colour2_l)
    {
        QString s;
        for (const auto& ch : str)
        {
            if (!ch.isSpace())
            {
                s.append(ch);
            }
        }
        if (s.left(2) == "R=")
        {
            Colour2_[0] = s.remove(0, 2).toStdString();
        }
        else if (s.left(2) == "G=")
        {
            Colour2_[1] = s.remove(0, 2).toStdString();
        }
        else if (s.left(2) == "B=")
        {
            Colour2_[2] = s.remove(0, 2).toStdString();
        }
        else if (s.left(2) == "A=")
        {
            Colour2_[3] = s.remove(0, 2).toStdString();
        }
        else
        {
            // just omit that
        }
    }
}

void MainWindow::customTemplatePre(Create_Image_Task* task)
{
    QString Formula = info.curr().layerFormula(0);
    std::string msg;
    std::vector<_var> post;
    if (!to_postorder(Formula.toStdString(), &msg, post, { "z", "x", "y", "z0", "x0", "y0", "t", "k" })) {
        QMessageBox::critical(this, "Error", "Syntax error in Customized Template Formula!");
        qDebug() << QString::fromStdString(msg);
        return;
    }
    task->setFormula(post);
}

bool MainWindow::createImagePre(Create_Image_Task* task)
{
//    QString Colour1 = ui->Convergent_Points_Colour_Formula->toPlainText();
//    QString Colour2 = ui->Divergent_Points_Colour_Formula->toPlainText();
    std::string Colour1_[4], Colour2_[4];

//    Read_RGBA(Colour1, Colour2, Colour1_, Colour2_);
    Colour1_[0] = info.curr().layerColor(0, "Con.R").toStdString();
    Colour1_[1] = info.curr().layerColor(0, "Con.G").toStdString();
    Colour1_[2] = info.curr().layerColor(0, "Con.B").toStdString();
    Colour1_[3] = info.curr().layerColor(0, "Con.A").toStdString();
    Colour2_[0] = info.curr().layerColor(0, "Div.R").toStdString();
    Colour2_[1] = info.curr().layerColor(0, "Div.G").toStdString();
    Colour2_[2] = info.curr().layerColor(0, "Div.B").toStdString();
    Colour2_[3] = info.curr().layerColor(0, "Div.A").toStdString();

    qDebug() << QString::fromLatin1(Colour1_[0]);

    std::string msg1[4], msg2[4];
    std::vector<_var> post1[4], post2[4];
    for (int i = 0; i != 4; i++) {
        if (!to_postorder(Colour1_[i], msg1 + i, post1[i], { "z", "x", "y", "z0", "x0", "y0", "t", "k" }))
        {
            QMessageBox::critical(this, "Error", "Syntax error in Convergent Point Colour Setting!");
            qDebug() << QString::fromStdString(msg1[i]);
            return false;
        }
    }
    for (int i = 0; i != 4; i++) {
        if (!to_postorder(Colour2_[i], msg2 + i, post2[i], { "z", "x", "y", "z0", "x0", "y0", "t", "k" }))
        {
            QMessageBox::critical(this, "Error", "Syntax error in Divergent Point Colour Setting!");
            qDebug() << QString::fromStdString(msg2[i]);
            return false;
        }
    }

    Create_Images_Task_Pre(task);
    return true;
}

void MainWindow::on_actionPreview_Refresh_triggered()
{
    Create_Image_Task* preview = new Create_Image_Task(this);

    if (!createImagePre(preview))
    {
        delete preview;
        return;
    }

    QString Pre_Img_Dir;


    QDir ck(QCoreApplication::applicationDirPath() + "/temp");
    if(!ck.exists())
    {
        ck.mkdir(ck.absolutePath());
    }
    Pre_Img_Dir = QCoreApplication::applicationDirPath() + "/temp";

    /*
    if(curr_info.template_ == 2)
    {
        double t = ui->doubleSpinBox_t->value();
        std::complex<double> c1 = curr_info.Julia_c1, c2 = curr_info.Julia_c2;
        double& k = curr_info.Julia_c_rate;
        preview->setTemplate2(_curr_complex(c1, c2, t, k));
    }
    else if(curr_info.template_ == 4)
    {
        double t = ui->doubleSpinBox_t->value();

        double& k = curr_info.Newton_c_rate;

        std::complex<double> arr[10];
        for(int i = 0; i != 10; i++)
        {
            arr[i] = _curr_complex(curr_info.Newton_xn_1[i], curr_info.Newton_xn_2[i], t, k);
        }

        preview->setTemplate4(_curr_complex(curr_info.Newton_a_1, curr_info.Newton_a_2, t, k),
                              arr,
                              _curr_complex(curr_info.Newton_sin_1, curr_info.Newton_sin_2, t, k),
                              _curr_complex(curr_info.Newton_cos_1, curr_info.Newton_cos_2, t, k),
                              _curr_complex(curr_info.Newton_ex_1, curr_info.Newton_ex_2, t, k));
    }
    else if(curr_info.template_ == 5)
    {

    }
    */

    customTemplatePre(preview);

    preview->setImage(info.curr().PreviewCentre("X"), info.curr().PreviewCentre("Y"),
                      info.curr().PreviewSize("X"), info.curr().PreviewSize("Y"),
                      info.curr().PreviewImageSize("X"), info.curr().PreviewImageSize("Y"),
                      info.curr().PreviewRotation(), info.curr().PreviewTime(),
                      "png", Pre_Img_Dir, "Preview Image", "Preview",
                      info.curr().inverseYAsis());

    QThreadPool::globalInstance()->start(preview);
    qDebug() << "Refreshed";
}

void MainWindow::getImage(QImage img)
{
    qDebug() << "Preview get image";
    this->image_preview = img;
    update();
    ui->Label_Preview_Image->resize(ui->Preview_Image->size());
    int preview_w_width(ui->Preview_Image->width());
    int Preview_w_height(ui->Preview_Image->height());
    double rate_w = static_cast<double>(Preview_w_height) / preview_w_width;
    double rate_i = static_cast<double>(image_preview.height()) / image_preview.width();
    if(rate_i > rate_w)
    {
        ui->Label_Preview_Image->setPixmap(QPixmap::fromImage(image_preview).scaledToHeight(Preview_w_height));
    }
    else
    {
        ui->Label_Preview_Image->setPixmap(QPixmap::fromImage(image_preview).scaledToWidth(preview_w_width));
    }

    ui->label_previewInVideo->resize(ui->widget_previewInVideo->size());
    int preview_w_width_video(ui->widget_previewInVideo->width());
    int Preview_w_height_video(ui->widget_previewInVideo->height());
    double rate_w_video = static_cast<double>(Preview_w_height_video) / preview_w_width_video;
    if(rate_i > rate_w_video)
    {
        ui->label_previewInVideo->setPixmap(QPixmap::fromImage(image_preview).scaledToHeight(Preview_w_height_video));
    }
    else
    {
        ui->label_previewInVideo->setPixmap(QPixmap::fromImage(image_preview).scaledToWidth(preview_w_width_video));
    }
}

void MainWindow::dealClose(QObject* sd)
{
    Q_UNUSED(sd);

    qDebug() << "dealClose";
    exit(0);
}

void MainWindow::updateProgressBar(double p)
{
    ui->progressBar_Preview->setValue(p);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if(!save_or_not)
    {
        int result = QMessageBox::question(this,
                                           "Information",
                                           "You haven't saved this project,\ndo you want to save it?",
                                           QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                                           QMessageBox::Yes);
        if(result == QMessageBox::Yes)
        {
            on_actionSave_S_triggered();
        }
        else if(result == QMessageBox::No)
        {
            qDebug() << "Do not save";
        }
        else
        {
            event->ignore();
            return;
        }
    }
    emit destroyed();
}

void MainWindow::iniTemplate(int n)
{
    qDebug() << "initialize template succeed";
    switch(n)
    {
    case 1:
        ui->Template_Choice_1->setChecked(true);
        break;
    case 2:
        ui->Template_Choice_2->setChecked(true);
        break;
    case 3:
        ui->Template_Choice_3->setChecked(true);
        break;
    case 4:
        ui->Template_Choice_4->setChecked(true);
        break;
    default:
        break;
    }
    Project_Template = QString::number(n);
}

void MainWindow::iniValue(double arg1, double arg2, int arg3)
{
    ui->Min_class_value->setValue(arg1);
    ui->Max_class_value->setValue(arg2);
    ui->Max_loop_time->setValue(arg3);
}

void MainWindow::iniSize(int arg1, int arg2)
{
    ui->Image_size_X->setValue(arg1);
    ui->Image_size_Y->setValue(arg2);
}

void MainWindow::iniRouteInfo(QVector<double> vec)
{
    for(int i = 0; i != vec.size(); i++)
    {
        table_route_model->setItem(i / 6, i % 6, new QStandardItem(QString::number(vec[i], 'g', 11)));
    }
}

void MainWindow::iniImagePath(QString arg)
{
    ui->lineEdit_imagePath->setText(arg);
}

void MainWindow::iniImagePrefix(QString arg)
{
    ui->lineEdit_imagePrefix->setText(arg);
}

void MainWindow::iniFrameRate(int arg)
{
    ui->comboBox_fps->setCurrentIndex(arg);
}

void MainWindow::iniTotalTime(QString arg)
{
    ui->timeEdit->setTime(QTime::fromString(arg, "mm:ss"));
}

void MainWindow::iniMusicAdded(QVector<QString> vec)
{
    for(const auto& line : vec)
    {
        ui->textBrowser_music->append(line + "\n");
    }
}

void MainWindow::on_Slider_t_valueChanged(int value)
{
    ui->doubleSpinBox_t->setValue(value / 1000000000.0);
}

void MainWindow::on_doubleSpinBox_t_editingFinished()
{
    ui->Slider_t->setValue(1000000000 * ui->doubleSpinBox_t->value());
    if (curr_info.ps.autoRefresh)
    {
        MainWindow::on_actionPreview_Refresh_triggered();
    }
}

void MainWindow::on_pushButton_routeAdd_clicked()
{
    int row_count_now = table_route_model->rowCount();
    for(int i = 0; i != 6; i++)
    {
        table_route_model->setItem(row_count_now, i, new QStandardItem("0"));
    }
}

void MainWindow::on_pushButton_routeRemove_clicked()
{
    table_route_model->removeRow(table_route_model->rowCount() - 1);
    edit();
    display();
}

void MainWindow::on_pushButton_routeClear_clicked()
{
    table_route_model->setRowCount(0);
    edit();
}

void MainWindow::on_actionCreate_Images_triggered()
{
    if(!ui->actionCreate_Images->isEnabled()) return;

    if(!save_or_not)
    {
        QMessageBox::warning(this, "Can not create images", "You have not saved the project.");
        return;
    }

    if(!isRouteValid)
    {
        QMessageBox::warning(this, "Can not create images", "The Route Settings are invalid.");
        return;
    }

    QString image_format = "png";
    QString path = ui->lineEdit_imagePath->text();
    QString name = ui->lineEdit_imagePrefix->text();
    if(path.isEmpty() || !QDir(path).exists())
    {
        QMessageBox::warning(this, "Can not create images", "The path does not exist.");
        return;
    }

    ui->actionStop->setDisabled(false);
    ui->actionCreate_Images->setDisabled(true);
    ui->actionCreate_Images_in_Range->setDisabled(true);
    ui->actionCheck_Images->setDisabled(true);

    qDebug() << ui->timeEdit->time().second() + 60 * ui->timeEdit->time().minute();
    int total_image = ui->comboBox_fps->currentText().toInt() * (ui->timeEdit->time().second() + 60 * ui->timeEdit->time().minute());
    int current_index = 0;
    int X = ui->Image_size_X->value();
    int Y = ui->Image_size_Y->value();

    create_image_info = new Create_Image_Info;
    connect(this, &MainWindow::build_image_info_signal, create_image_info, &Create_Image_Info::set_info);
    connect(this, &MainWindow::build_image_updateInfo_signal, create_image_info, &Create_Image_Info::updateInfo);
    create_image_info->show();
    emit build_image_info_signal(path + "/" + name, image_format, total_image, 0);

    for(int i = 0; i < total_image - 1; i++)
    {
        double T = static_cast<double>(i) / total_image;
        while(current_index + 2 < table_route_model->rowCount() && T > Tb(current_index + 1, 0))
        {
            current_index++;
        }
        double w1 = Tb(current_index, 4);
        double w2 = Tb(current_index + 1, 4);
        double t1 = Tb(current_index, 0);
        double t2 = Tb(current_index + 1, 0);
        double x1 = Tb(current_index, 1);
        double x2 = Tb(current_index + 1, 1);
        double y1 = Tb(current_index, 2);
        double y2 = Tb(current_index + 1, 2);
        double k  = Tb(current_index, 5);
        double t  = (T - t1) / (t2 - t1);
        double x, y;

        double angle = Tb(current_index, 3)
                + (Tb(current_index + 1, 3) - Tb(current_index, 3))
                * ((1 - k) * t + k * pow(t, 2));
        double width = 1 / (t * (1 / w2) + (1 - t) * (1 / w1));

        if(fabs(1 - w1 / w2) < 1E-5)
        {
            x = x1 + t * x2 + (1 - t) * x1;
            y = y1 + t * y2 + (1 - t) * y1;
        }
        else
        {
            x = x1 + (x2 - x1) / log(w1 / w2) * log((w1 / w2 - 1) * t + 1);
            y = y1 + (y2 - y1) / log(w1 / w2) * log((w1 / w2 - 1) * t + 1);
        }

        // qDebug() << t << current_index << x << y << width << angle;
        Create_Image_Task* create_images = new Create_Image_Task(this);
        if (!createImagePre(create_images))
        {
            delete create_images;
            return;
        }
        if(curr_info.template_ == 2)
        {
            std::complex<double> c1 = curr_info.Julia_c1, c2 = curr_info.Julia_c2;
            double k = curr_info.Julia_c_rate;
            create_images->setTemplate2(c1 + (c2 - c1) * ((1 - k) * T + k * T * T));
        }
        if(curr_info.template_ == 4)
        {
            double& k = curr_info.Newton_c_rate;

            std::complex<double> arr[10];
            for(int i = 0; i != 10; i++)
            {
                arr[i] = _curr_complex(curr_info.Newton_xn_1[i], curr_info.Newton_xn_2[i], T, k);
            }

            create_images->setTemplate4(_curr_complex(curr_info.Newton_a_1, curr_info.Newton_a_2, T, k),
                                        arr,
                                        _curr_complex(curr_info.Newton_sin_1, curr_info.Newton_sin_2, T, k),
                                        _curr_complex(curr_info.Newton_cos_1, curr_info.Newton_cos_2, T, k),
                                        _curr_complex(curr_info.Newton_ex_1, curr_info.Newton_ex_2, T, k));

        }
        create_images->setImage(x, y, width, width * Y / X, X, Y, angle, T, image_format, path, name + QString::number(i), "Create_Image", curr_info.y_inverse);
        QThreadPool::globalInstance()->start(create_images);
    }

    Create_Image_Task* create_images = new Create_Image_Task(this);
    // create_images->setAutoDelete(false);

    createImagePre(create_images);
    if(curr_info.template_ == 2)
    {
        create_images->setTemplate2(curr_info.Julia_c2);
    }
    if(curr_info.template_ == 4)
    {
        create_images->setTemplate4(curr_info.Newton_a_2,
                                    curr_info.Newton_xn_2,
                                    curr_info.Newton_sin_2,
                                    curr_info.Newton_cos_2,
                                    curr_info.Newton_ex_2);
    }
    create_images->setImage(Tb(table_route_model->rowCount() - 1, 1),
                            Tb(table_route_model->rowCount() - 1, 2),
                            Tb(table_route_model->rowCount() - 1, 4),
                            Tb(table_route_model->rowCount() - 1, 4) * Y / X,
                            X, Y,
                            Tb(table_route_model->rowCount() - 1, 3),
                            1, image_format, path, name + QString::number(total_image - 1), "Create_Image_Last",
                            curr_info.y_inverse);
    QThreadPool::globalInstance()->start(create_images);
    qDebug() << "Here Here !!!!";
}

void MainWindow::on_commandLinkButton_Image_clicked()
{
    MainWindow::on_actionCreate_Images_triggered();
}

void MainWindow::on_Slider_t_sliderReleased()
{
    if(curr_info.ps.autoRefresh)
    {
        MainWindow::on_actionPreview_Refresh_triggered();
    }
}

void MainWindow::on_actionStop_triggered()
{
    if(!ui->actionStop->isEnabled())
    {
        QMessageBox::information(this, "Information", "You have not created images.");
        return;
    }
    emit createImageStop();

    create_image_info->close();

    qDebug() << "Build Thread quit";
    ui->actionCreate_Images->setDisabled(false);
    ui->actionCreate_Images_in_Range->setDisabled(false);
    ui->actionCheck_Images->setDisabled(false);
    ui->actionStop->setDisabled(true);
}

void MainWindow::build_image_finished_deal()
{
    ui->actionCreate_Images->setDisabled(false);
    ui->actionCreate_Images_in_Range->setDisabled(false);
    ui->actionCheck_Images->setDisabled(false);
    ui->actionStop->setDisabled(true);
    emit build_image_updateInfo_signal();
}

void MainWindow::build_image_one_ok()
{
    emit build_image_updateInfo_signal();
}

void MainWindow::on_toolButton_imagePath_clicked()
{  
    QString default_dir = QFileInfo(Open_Location).filePath();
    QString Pro_Path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Choose Path"), QDir::fromNativeSeparators(default_dir)));
    ui->toolButton_imagePath->setDisabled(false);
    ui->lineEdit_imagePath->setText(Pro_Path);
}

void MainWindow::on_actionCreate_Video_triggered()
{

#if !(defined (WIN32) || defined (_WIN64) || defined (__linux__))
    UNSUPPORTED_PLATFORM;
    return;
#endif

    if(!save_or_not)
    {
        QMessageBox::information(this, "Information", "You have not saved the project.");
        return;
    }

    // QMessageBox::information(this, "Information", "Creating video...", QMessageBox::NoButton);

    int crf_value = 18;
    QString video_file_name = ui->lineEdit_videoName->text();
    QString video_file_path = ui->lineEdit_videoPath->text(); // only used in high version
    QString video_format = "mp4";
#if defined (WIN32) || defined (WIN64)
    QString ffmpeg_arg1 = QString("\"") + QCoreApplication::applicationDirPath() + "\\Resources\\ffmpeg.exe\" -r ";
#elif defined (__linux__)
    QString PowerShell_arg1 = "ffmpeg -r ";
#endif
    ffmpeg_arg1.append(ui->comboBox_fps->currentText());
    ffmpeg_arg1.append(" -f image2 -i ");
    ffmpeg_arg1.append(tr("\"") + ui->lineEdit_imagePath->text() + "/" + ui->lineEdit_imagePrefix->text());
    ffmpeg_arg1.append("%d.png\" -vcodec libx264 -crf ");
    ffmpeg_arg1.append(QString::number(crf_value));
    ffmpeg_arg1.append(" -pix_fmt yuv420p ");
    ffmpeg_arg1.append(ui->textBrowser_music->toPlainText().isEmpty() ?
                               video_file_name + "." + video_format
                             : video_file_name + "_temp." + video_format);

    ui->statusbar->showMessage(tr("Creating Video..."), 300000);

    bool Alive[3] {false, false, false};
    QFile video(video_file_path + "/" + video_file_name + "." + video_format);
    if(video.exists())
    {
        if(QMessageBox::question(this, "Choice", tr("The video ") + video_file_name + "." + video_format
                                 + " already exists.\nDo you want to overwrite it?") == QMessageBox::Yes)
        {
            video.remove();
        }
        else
        {
            ui->statusbar->showMessage("", 30000);
            return;
        }
    }

    qDebug() << "Creating Video...";

    QProcess* create_video = new QProcess(this);
    create_video->setWorkingDirectory(QDir::fromNativeSeparators(video_file_path));
#if defined (WIN32) || defined (_WIN64)
    qDebug() << ffmpeg_arg1;
    create_video->start(ffmpeg_arg1);
    QString file_suffix = "cmd";
#elif defined(__linux__)
    create_video->start(ffmpeg_arg1);
    QString file_suffix = "sh";
#endif
    Alive[0] = create_video->waitForFinished(300000);
    qDebug() << create_video->readAllStandardOutput();

    if(!Alive[0])
    {
        QFile create_video_ps(video_file_path + "/Create_Video." + file_suffix);
        create_video_ps.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out1(&create_video_ps);
        out1 << ffmpeg_arg1;
        create_video_ps.close();

        // Retry running shell file
        QProcess* run_sh = new QProcess(this);
        run_sh->setWorkingDirectory(QDir::fromNativeSeparators(video_file_path));
#if defined (WIN32) || (_WIN64)
        run_sh->start("Create_Video.cmd");
#elif defined (__linux__)
        run_sh->start("/bin/sh", QStringList() << "Create_Video.sh");
#endif
        Alive[0] = run_sh->waitForFinished(300000);
        if(Alive[0])
        {
            QFile ck(video_file_path + "/" +
                        (ui->textBrowser_music->toPlainText().isEmpty() ?
                         video_file_name + "." + video_format
                       : video_file_name + "_temp." + video_format));
            if(ck.exists())
            {
                QFile::remove(video_file_path + "/Create_Video." + file_suffix);
            }
            else
            {
                Alive[0] = false;
            }
        }
    }

    if(ui->textBrowser_music->toPlainText().isEmpty())
    {
        if(Alive[0])
        {
            QMessageBox::information(this, "Information", video_file_path + "/" + video_file_name + "." + video_format
                                           + "\nVideo Creating Finished!");
            ui->statusbar->showMessage(tr("Video Creating Finished!"), 5000);
            return;
        }
        else
        {
            ui->statusbar->showMessage(tr("Video Creating Failed"), 5000);
            QMessageBox::warning(this, "Error Information", "Video Creation failed!\nYou can run Create_Video." + file_suffix + " code instead.");
        }
        return;
    }

    ui->statusbar->showMessage(tr("Processing Music..."), 300000);

    QFile Music_Added_NoEnd_(video_file_path + "/Music_Added_NoEnd.txt");
    Music_Added_NoEnd_.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&Music_Added_NoEnd_);
    if(!curr_info.music_list.empty())
    {
        for(int i = 1; i <= curr_info.music_list.size(); i++)
        {
            QFile::remove(video_file_path + "/" + QString::number(i) + ".mp3");
            if(QFile::copy(curr_info.music_list[i - 1], video_file_path + "/" + QString::number(i) + ".mp3"))
            {
                out << "file '" << QString::number(i) + ".mp3'" << Qt::endl;
            }
        }
        Music_Added_NoEnd_.close();
    }
#if defined (WIN32) || (WIN64)
    QString ffmpeg_arg2 = QString("\"") + QCoreApplication::applicationDirPath() + "\\Resources\\ffmpeg.exe\" -f concat -i Music_Added_NoEnd.txt -c copy BGM.mp3";
#elif defined (__linux__)
    QString ffmpeg_arg2 = "ffmpeg -f concat -i Music_Added_NoEnd.txt -c copy BGM.mp3";
#endif
    qDebug() << ffmpeg_arg2;
    create_video->start(ffmpeg_arg2);
    Alive[1] = create_video->waitForFinished(300000);

    ui->statusbar->showMessage(tr("Adding Music..."), 300000);

#if defined (WIN32) || (WIN64)
    QString ffmpeg_arg3 = QString("\"") + QCoreApplication::applicationDirPath() + "\\Resources\\ffmpeg.exe\" -i BGM.mp3 -i ";
#elif defined (__linux__)
    QString PowerShell_arg3 = "ffmpeg -i BGM.mp3 -i ";
#endif
    ffmpeg_arg3.append(video_file_name + "_temp." + video_format);
    ffmpeg_arg3.append(" -shortest -f mp4 ");
    ffmpeg_arg3.append(video_file_name + "." + video_format);
    if(Alive[0] && Alive[1])
    {
        qDebug() << ffmpeg_arg3;
        create_video->start(ffmpeg_arg3);
        Alive[2] = create_video->waitForFinished(60000);
        QFile ck_result(video_file_path + "/" + video_file_name + "." + video_format);
        if(ck_result.exists())
        {
            QFile::remove(video_file_path + "/" + video_file_name + "_temp." + video_format);
            QFile::remove(video_file_path + "/BGM.mp3");
            QFile::remove(video_file_path + "/Music_Added_NoEnd.txt");
            for(int i = 1; i <= curr_info.music_list.size(); i++)
            {
                QFile::remove(video_file_path + "/" + QString::number(i) + ".mp3");
            }
        }
        else
        {
            Alive[2] = false;
        }
    }

    if(!Alive[2])
    {
        QFile add_music_ps(video_file_path + "/Add_Music." + file_suffix);
        add_music_ps.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out2(&add_music_ps);
        out2 << ffmpeg_arg2 << Qt::endl;
        out2 << ffmpeg_arg3;
        add_music_ps.close();
        // Retry running shell file
        QProcess* run_sh = new QProcess(this);
        run_sh->setWorkingDirectory(QDir::fromNativeSeparators(video_file_path));

        if(Alive[0])
        {
#if defined (WIN32) || (_WIN64)
            run_sh->start("Add_Music.cmd");
#elif defined (__linux__)
            run_sh->start("/bin/sh", QStringList() << "Add_Music.sh");
#endif
            Alive[2] = run_sh->waitForFinished(300000);
            if(Alive[2])
            {
                QFile ck(video_file_path + "/" + video_file_name + "." + video_format);
                if(ck.exists())
                {
                    QFile::remove(video_file_path + "/Create_Video." + file_suffix);
                    QFile::remove(video_file_path + "/" + video_file_name + "_temp." + video_format);
                    QFile::remove(video_file_path + "/BGM.mp3");
                    QFile::remove(video_file_path + "/Music_Added_NoEnd.txt");
                    for(int i = 1; i <= curr_info.music_list.size(); i++)
                    {
                        QFile::remove(video_file_path + "/" + QString::number(i) + ".mp3");
                    }
                }
                else
                {
                    Alive[2] = false;
                }
            }
            if(!Alive[2])
            {
                ui->statusbar->showMessage(tr("Video Creating Failed"), 5000);
                QMessageBox::warning(this, "Error Information", "Video Creation failed!\nYou can run Add_Music." + file_suffix + " code instead.");
            }
        }
        else
        {
            ui->statusbar->showMessage(tr("Video Creating Failed"), 5000);
            QMessageBox::warning(this, "Error Information", "Video Creation failed!\nFirst run Create_Video." + file_suffix
                                       + ",\nsecond run Add_Music." + file_suffix + ".");
        }
    }

    if(Alive[2])
    {
        ui->statusbar->showMessage(tr("Video Creating Finished!"), 5000);
        QMessageBox::information(this, "Information", video_file_path + "/" + video_file_name + "." + video_format
                                       + "\nVideo Creating Finished!");
    }
}

void MainWindow::on_toolButton_videoPath_clicked()
{
    QString default_dir = QDir::homePath();
    QString video_Path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Choose Path"), QDir::fromNativeSeparators(default_dir)));
    ui->toolButton_imagePath->setDisabled(false);
    if(video_Path == "") return;
    ui->lineEdit_videoPath->setText(video_Path);
    edit();
}

void MainWindow::on_actionEnglish_triggered()
{
    // Help* help = new Help;
    // help->show();
    QDesktopServices::openUrl(QUrl(English_Help_Url));
}

void MainWindow::on_MainWindow_HelpEnglish_clicked()
{
    MainWindow::on_actionEnglish_triggered();
}

void MainWindow::on_pushButton_addMusic_clicked()
{
    QString Music_File_Name = QFileDialog::getOpenFileName(this,
                                                    tr("Choose Music File"),
                                                    QDir::currentPath(),
                                                    tr("Music Files(*mp3)"));
    QFile check(Music_File_Name);
    if(check.exists())
    {
        ui->textBrowser_music->append(Music_File_Name);
        buff_info.music_list.push_back(Music_File_Name);
        edit(EDIT_ALREADY);
    }
    else if(Music_File_Name != "")
    {
        QMessageBox::warning(this, "Music Adding Error", "The file doesn't exist!");
    }
}

void MainWindow::on_pushButton_deleteMusic_clicked()
{
    ui->textBrowser_music->clear();
    buff_info.music_list.clear();
    edit(EDIT_ALREADY);
}

void MainWindow::on_pushButton_UniformMotion_clicked()
{
    if(!isRouteValid)
    {
        QMessageBox::warning(this, "Fail to set uniform motion", "The route is invalid.");
        return;
    }
    NO_EDIT = true;
    int total_index = curr_info.Route_.size();
    double* L = new double[total_index - 1];
    double* L_till = new double[total_index - 1];
    double total_L = 0;
    for(int current_index = 0; current_index != total_index - 1; current_index++)
    {
        double w1 = Tb(current_index, 4);
        double w2 = Tb(current_index + 1, 4);
        if(w1 * w2 <= 0)
        {
            QMessageBox::critical(this, "Fail to set uniform motion", "Mathematical Error!\nIllegal Width!");
            display();
            return;
        }

        double x1 = Tb(current_index, 1);
        double x2 = Tb(current_index + 1, 1);
        double y1 = Tb(current_index, 2);
        double y2 = Tb(current_index + 1, 2);
        double dL = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
        if(fabs(1 - w1 / w2) < 1E-5)
        {
            L[current_index] = (1 / (2 * w1) + 1 / (2 * w2)) * dL;
        }
        else
        {
            L[current_index] = (1 / w2 - 1 / w1) / log(w1 / w2) * dL;
        }
        L_till[current_index] = total_L;
        total_L += L[current_index];
    }
    if(total_L == 0)
    {
        QMessageBox::critical(this, "Fail to set uniform motion", "Mathematical Error!\nTotal length equals to 0!");
        display();
        return;
    }
    for(int current_index = 0; current_index != total_index - 1; current_index++)
    {
        SetTb(current_index, 0, L_till[current_index] / total_L);
        SetTb(current_index, 5, 0);
    }
    SetTb(total_index - 1, 0, 1);
    SetTb(total_index - 1, 5, 0);
    delete[] L;
    delete[] L_till;

    NO_EDIT = false;
    edit();
}

void MainWindow::on_actionFFmpeg_triggered()
{
#if defined (WIN32) || defined (_WIN64)
    QProcess FFmpeg_Process(this);
    QString programme = "Resources/ffmpeg.exe";
    // run the programme FFmpeg
    FFmpeg_Process.start(programme);
    if(FFmpeg_Process.waitForFinished(5000))
    {
        QMessageBox::information(this, "Information", "FFmpeg already installed!");
    }
    else
    {
        QMessageBox::warning(this, "Information", "Dependency ffmpeg.exe fails!");
    }
#elif defined (__linux__)
    if (!QDesktopServices::openUrl(QUrl("http://www.ffmpeg.org/download.html#build-linux")))
    {
        QMessageBox::warning(this, "Information", "Can not access the web site.");
    }
#else
    UNSUPPORTED_PLATFORM;
#endif
}

void MainWindow::on_actionCheck_Update_triggered()
{
    QNetworkRequest quest;
    // fetch online update information
    quest.setUrl(QUrl("https://frd.teddy-van-jerry.org/FRD-Maintenance/software_update.json"));
    quest.setHeader(QNetworkRequest::UserAgentHeader, "RT-Thread ART");
    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(quest);
    QEventLoop event;
    connect(response, SIGNAL(finished()), &event, SLOT(quit()));
    event.exec();
    QString json_content = response->readAll();
    qDebug() << json_content;

    QJsonDocument doc = QJsonDocument::fromJson(json_content.toUtf8());

    //get the jsonObject
    QJsonObject jUpdateInfo = doc.object();
#if defined (WIN32) || defined (WIN64)
    QJsonValue value = jUpdateInfo.value(QString("Windows"));
#elif defined(_linux_)
    QJsonValue value = jUpdateInfo.value(QString("Linux"));
#endif
    QString Latest_Version = value["LatestVersion"].toString();
    QString Update_Info = "Latest version is " + Latest_Version;
    QMessageBox::information(this, "Update Info", Update_Info);
}

void MainWindow::edit(int mode) // default as EDIT_HERE
{
    if(NO_EDIT) return;

    save_or_not = false;

    qDebug() << "High Version Edit:";

    Info_Save temp_info = curr_info;

    if(redo_max_depth != _MAX_SAVE_) redo_max_depth++;

    if(current_info_v == 0)
    {
        for(int i = redo_max_depth - 1; i > 0; i--)
        {
            pre_info[i] = pre_info[i - 1];
        }
    }
    else if(current_info_v > 1)
    {
        for(int i = current_info_v; i + 1 < redo_max_depth; i++)
        {
            pre_info[i + 1 - current_info_v] = pre_info[i];
        }
    }
    redo_max_depth -= current_info_v;
    qDebug() << "Previous info_v is " << current_info_v;
    qDebug() << "Current redo_max_depth is " << redo_max_depth << "\n";
    current_info_v = 0;

    curr_info = temp_info;

    if(mode == EDIT_HERE)
    {
        // Template
        if      (Project_Template == "1") curr_info.template_ = 1;
        else if (Project_Template == "2") curr_info.template_ = 2;
        else if (Project_Template == "3") curr_info.template_ = 3;
        else if (Project_Template == "4") curr_info.template_ = 4;
        else if (Project_Template == "5") curr_info.template_ = 5;

        // Customized Formula
        curr_info.CustomFormula_ = ui->lineEdit_Custom_Formula->text();

        // Image Value
        curr_info.min_class_v = ui->Min_class_value->value();
        curr_info.max_class_v = ui->Max_class_value->value();
        curr_info.max_loop_t  = ui->Max_loop_time->value();

        // Route Info
        QList<Route_Info> Route;
        for(int i = 0; i != table_route_model->rowCount(); i++)
        {
            Route_Info temp {Tb(i, 0), Tb(i, 1), Tb(i, 2), Tb(i, 3), Tb(i, 4), Tb(i, 5)};
            Route.push_back(temp);
        }
        curr_info.setRouteInfo(Route);

        // Image Other
        curr_info.image_size_x = ui->Image_size_X->value();
        curr_info.image_size_y = ui->Image_size_Y->value();
        curr_info.frame_rate_index = ui->comboBox_fps->currentIndex();
        curr_info.total_time_str = ui->timeEdit->time().toString("mmss");
        curr_info.img_path = ui->lineEdit_imagePath->text();
        curr_info.img_prefix = ui->lineEdit_imagePrefix->text();

        // Colour Info
        curr_info.setColourInfo(ui->Convergent_Points_Colour_Formula->toPlainText(), true);
        curr_info.setColourInfo(ui->Divergent_Points_Colour_Formula->toPlainText(), false);

        // Video Info
        // TODO: use setXXX function instead
        curr_info.video_path = ui->lineEdit_videoPath->text();
        curr_info.video_name = ui->lineEdit_videoName->text();

        // ===== Last ====
        buff_info = curr_info;
    }
    else // EDIT_ALREADY
    {
        curr_info = buff_info;
    }

    ui->actionUndo->setEnabled(true);
}

void MainWindow::display()
{
    NO_EDIT = true;
/*
    qDebug() << "   =====Display=====";
    qDebug() << "   Current info_v " << current_info_v;
    qDebug() << "   Currnet redo_max_depth " << redo_max_depth;
    qDebug() << "   =================";

    switch(curr_info.template_)
    {
    case 1:
        ui->Template_Choice_1->setChecked(true); Project_Template = "1";
        break;
    case 2:
        ui->Template_Choice_2->setChecked(true); Project_Template = "2";
        break;
    case 3:
        ui->Template_Choice_3->setChecked(true); Project_Template = "3";
        break;
    case 4:
        ui->Template_Choice_4->setChecked(true); Project_Template = "4";
        break;
    case 5:
        ui->Template_Choice_5->setChecked(true); Project_Template = "5";
        break;
    default:
        // uncheck all
        ui->Template_Choice_1->setAutoExclusive(false);
        ui->Template_Choice_2->setAutoExclusive(false);
        ui->Template_Choice_3->setAutoExclusive(false);
        ui->Template_Choice_4->setAutoExclusive(false);
        ui->Template_Choice_5->setAutoExclusive(false);
        ui->Template_Choice_1->setChecked(false);
        ui->Template_Choice_2->setChecked(false);
        ui->Template_Choice_3->setChecked(false);
        ui->Template_Choice_4->setChecked(false);
        ui->Template_Choice_5->setChecked(false);
        ui->Template_Choice_1->setAutoExclusive(true);
        ui->Template_Choice_2->setAutoExclusive(true);
        ui->Template_Choice_3->setAutoExclusive(true);
        ui->Template_Choice_4->setAutoExclusive(true);
        ui->Template_Choice_5->setAutoExclusive(true);
        Project_Template = "Undefined";
        break;
    }

    // Image Value
    ui->Min_class_value->setValue(curr_info.min_class_v);
    ui->Max_class_value->setValue(curr_info.max_class_v);
    ui->Max_loop_time->setValue(curr_info.max_loop_t);
    ui->checkBox_yInverse->setChecked(curr_info.y_inverse);

    // Colour Info
    ui->Convergent_Points_Colour_Formula->setPlainText(curr_info.Colour1_f);
    ui->Divergent_Points_Colour_Formula->setPlainText(curr_info.Colour2_f);

    // Route Info
    isRouteValid = true;
    int preRowCount = table_route_model->rowCount();
    table_route_model->setRowCount(curr_info.Route_.size());
    for(int i = preRowCount; i < curr_info.Route_.size(); i++)
    {
        for(int j = 0; j != 6; j++)
        {
            table_route_model->setItem(i, j, new QStandardItem("0"));
        }
    }

    // qDebug() << model->rowCount();
    for(int i = 0; i != curr_info.Route_.size(); i++)
    {
        SetTb(i, 0, curr_info.Route_[i].t);
        SetTb(i, 1, curr_info.Route_[i].x);
        SetTb(i, 2, curr_info.Route_[i].y);
        SetTb(i, 3, curr_info.Route_[i].angle);
        SetTb(i, 4, curr_info.Route_[i].width);
        SetTb(i, 5, curr_info.Route_[i].v_rate);
        if(i == 0 && Tb(0, 0) != 0)
        {
            table_route_model->item(0, 0)->setForeground(QBrush(Qt::red));
            table_route_model->item(0, 0)->setToolTip("The first t should be 0.");
            isRouteValid = false;
        }
        else if(i == curr_info.Route_.size() - 1 && Tb(i, 0) != 1)
        {
            table_route_model->item(i, 0)->setForeground(QBrush(Qt::red));
            table_route_model->item(i, 0)->setToolTip("The last t should be 1.");
            isRouteValid = false;
        }
        else if(i != 0 && Tb(i - 1, 0) > Tb(i, 0))
        {
            table_route_model->item(i, 0)->setForeground(QBrush(Qt::red));
            table_route_model->item(i, 0)->setToolTip("It is not in order.");
            isRouteValid = false;
        }
        else
        {
            table_route_model->item(i, 0)->setForeground(QBrush(Qt::black));
            table_route_model->item(i, 0)->setToolTip("");
        }

        if(Tb(i, 4) <= 0)
        {
            table_route_model->item(i, 4)->setForeground(QBrush(Qt::red));
            table_route_model->item(i, 4)->setToolTip("It should be larger than 0");
            isRouteValid = false;
        }
        else
        {
            table_route_model->item(i, 4)->setForeground(QBrush(Qt::black));
            table_route_model->item(i, 4)->setToolTip("");
        }
    }
    if(curr_info.Route_.size() == 0) isRouteValid = false;

    // Image Other
    ui->Image_size_X->setValue(curr_info.image_size_x);
    ui->Image_size_Y->setValue(curr_info.image_size_y);
    ui->comboBox_fps->setCurrentIndex(curr_info.frame_rate_index);
    ui->timeEdit->setTime(QTime::fromString(curr_info.total_time_str, "mmss"));
    ui->lineEdit_imagePath->setText(curr_info.img_path);
    ui->lineEdit_imagePrefix->setText(curr_info.img_prefix);

    // Customized Formula
    ui->lineEdit_Custom_Formula->setText(curr_info.CustomFormula_);

    // Colour Info
    // ui->Convergent_Points_Colour_Formula->setPlainText(curr_info.Colour1_f);
    // ui->Divergent_Points_Colour_Formula->setPlainText(curr_info.Colour2_f);

    // Video Info
    ui->lineEdit_videoPath->setText(curr_info.video_path);
    ui->lineEdit_videoName->setText(curr_info.video_name);
     ui->textBrowser_music->clear();
    for(const auto& c : curr_info.music_list)
    {
        ui->textBrowser_music->append(c);
    }

    // Preview Info
    preview_setting->updateInfo();
*/

    editor->setText(info.curr().text());
    NO_EDIT = false;
}

void MainWindow::on_actionUndo_triggered()
{
    if(!ui->actionUndo->isEnabled()) return;

    NO_EDIT = true;

    qDebug() << "High Version Undo:";
    qDebug() << "Previous info_v is " << current_info_v;

    if(current_info_v < redo_max_depth - 1)
    {
        current_info_v++;
        display();
        ui->actionRedo->setDisabled(false);
        if(current_info_v == redo_max_depth - 1)
        {
            ui->actionUndo->setDisabled(true);
        }
    }

    qDebug() << "Current info_v is " << current_info_v;
    qDebug() << "Current redo_max_depth is " << redo_max_depth << "\n";

    buff_info = curr_info;

    NO_EDIT = false;
}

void MainWindow::on_actionRedo_triggered()
{
    if(!ui->actionUndo->isEnabled()) return;

    NO_EDIT = true;

    qDebug() << "High Version Redo:";
    qDebug() << "Previous info_v is " << current_info_v;

    if(current_info_v > 0)
    {
        current_info_v--;
        display();
        ui->actionUndo->setDisabled(false);
        if(current_info_v == 0)
        {
            ui->actionRedo->setDisabled(true);
        }
    }

    qDebug() << "Current info_v is " << current_info_v;
    qDebug() << "Current redo_max_depth is " << redo_max_depth << "\n";

    buff_info = curr_info;

    NO_EDIT = false;
}

void MainWindow::on_Template_Choice_1_clicked()
{
    edit();
}

void MainWindow::on_Template_Choice_2_clicked()
{
    edit();
}

void MainWindow::on_Template_Choice_3_clicked()
{
    edit();
}

void MainWindow::on_Template_Choice_4_clicked()
{
    edit();
}

void MainWindow::on_Template_Choice_5_clicked()
{
    edit();
}

void MainWindow::routeEdit(QStandardItem* it)
{
    if(NO_EDIT) return;
    // NO_EDIT = true;
    bool ok;
    double value = it->text().toDouble(&ok);
    if(!ok) // not a legal number
    {
        QMessageBox::warning(this, "Route Setting Error", "The content should be a number");
        it->setData(QVariant(0.0), Qt::EditRole);
        return;
    }
    it->setData(QVariant(value), Qt::EditRole);
    // edit();
    // qDebug() << "SIGNAL - routeEdit";

    QList<Route_Info> model_list;
    for(int i = 0; i != table_route_model->rowCount(); i++)
    {
        if(!table_route_model->item(i, 5)) return;
        Route_Info temp_info {Tb(i, 0), Tb(i, 1), Tb(i, 2), Tb(i, 3), Tb(i, 4), Tb(i, 5)};
        model_list.push_back(temp_info);
    }
    if(model_list != curr_info.Route_)
    {
        qDebug() << "routeEdit";
        edit();
        display(); // used to check its validity
    }
    // NO_EDIT = false;
}

bool MainWindow::isColourAutoSaved()
{
    return ui->actionColour_Definition_Auto_Save->isChecked();
}

void MainWindow::saveElsewhere()
{
    on_actionSave_S_triggered();
}

void MainWindow::on_Min_class_value_editingFinished()
{
    if(ui->Min_class_value->value() == curr_info.min_class_v) return;
    edit();
}

void MainWindow::on_Max_class_value_editingFinished()
{
    if(ui->Max_class_value->value() == curr_info.max_class_v) return;
    edit();
}

void MainWindow::on_Max_loop_time_editingFinished()
{
    if(ui->Max_loop_time->value() == curr_info.max_loop_t) return;
    edit();
}

void MainWindow::dealRouteSort(int)
{
    edit();
    display(); // used to check the validity
}

void MainWindow::on_Image_size_X_editingFinished()
{
    if(ui->Image_size_X->value() == curr_info.image_size_x) return;
    edit();
}

void MainWindow::on_Image_size_Y_editingFinished()
{
    if(ui->Image_size_Y->value() == curr_info.image_size_y) return;
    edit();
}

void MainWindow::on_comboBox_fps_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    edit();
}

void MainWindow::on_timeEdit_editingFinished()
{
    if(ui->timeEdit->time().toString("mmss") == curr_info.total_time_str) return;
    edit();
}

void MainWindow::on_lineEdit_imagePath_editingFinished()
{
    if(ui->lineEdit_imagePath->text() == curr_info.img_path) return;
    edit();
}

void MainWindow::on_lineEdit_imagePrefix_editingFinished()
{
    if(ui->lineEdit_imagePrefix->text() == curr_info.img_prefix) return;
    edit();
}

void MainWindow::on_lineEdit_videoPath_editingFinished()
{
    if(ui->lineEdit_videoPath->text() == curr_info.video_path) return;
    edit();
}

void MainWindow::on_lineEdit_videoName_editingFinished()
{
    if(ui->lineEdit_videoName->text() == curr_info.video_name) return;
    edit();
}

void MainWindow::on_actionVersion_1_triggered()
{
    QDesktopServices::openUrl(QUrl("https://www.bilibili.com/video/BV1TU4y147ta"));
}

void MainWindow::on_actionAbout_Teddy_van_Jerry_triggered()
{
    on_MainWindow_AboutTVJ_clicked();
}

void MainWindow::on_actionVersion_2_triggered()
{
    QDesktopServices::openUrl(QUrl("https://www.bilibili.com/video/BV1qX4y1P74d"));
}

void MainWindow::on_actionBug_Report_triggered()
{
    QDesktopServices::openUrl(QUrl(Bug_Report));
}

void MainWindow::on_actionVersion_triggered()
{
    Version* version_dialog = new Version(this);
    version_dialog->show();
}

void MainWindow::Version_Dialog_Support(int index)
{
    switch(index)
    {
    case 1:
        on_actionEnglish_triggered();
        break;
    case 2:
        on_actionNew_Features_triggered();
        break;
    case 3:
        on_actionCheck_Update_triggered();
        break;
    default:
        break;
    }
}

void MainWindow::on_actionNew_Features_triggered()
{
    New_Features* new_features_dialog = new New_Features(this);
    new_features_dialog->show();
}

void MainWindow::on_pushButton_Min_class_default_clicked()
{
    switch(curr_info.template_)
    {
    case 1:
        if(curr_info.min_class_v == 0.25) return;
        ui->Min_class_value->setValue(0.25);
        edit();
        break;
    case 2:
        if(curr_info.min_class_v == 0.105) return;
        ui->Min_class_value->setValue(0.105);
        edit();
        break;
    case 4:
        if(curr_info.min_class_v == 1E-6) return;
        ui->Min_class_value->setValue(1E-6);
        edit();
        break;
    case 3:
        if(curr_info.min_class_v == 1E-3) return;
        ui->Min_class_value->setValue(1E-3);
        edit();
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_Max_value_default_clicked()
{
    switch(curr_info.template_)
    {
    case 1: case 2: case 3:
        if(curr_info.max_class_v == 2) return;
        ui->Max_class_value->setValue(2);
        edit();
        break;
    case 4:
        if(curr_info.max_class_v == 1E7) return;
        ui->Max_class_value->setValue(1E7);
        edit();
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_Max_loop_default_clicked()
{
    switch(curr_info.template_)
    {
    case 1: case 3:
        if(curr_info.max_loop_t == 50) return;
        ui->Max_loop_time->setValue(50);
        edit();
        break;
    case 2:
        if(curr_info.max_loop_t == 250) return;
        ui->Max_loop_time->setValue(250);
        edit();
        break;
    case 4:
        if(curr_info.max_loop_t == 2500) return;
        ui->Max_loop_time->setValue(2500);
        edit();
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_IV_clear1_clicked()
{
    switch(curr_info.template_)
    {
    case 1:
        if(curr_info.max_class_v == 0) return;
        ui->Min_class_value->setValue(0);
        edit();
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_IV_clear2_clicked()
{
    switch(curr_info.template_)
    {
    case 1:
        if(curr_info.min_class_v == 0) return;
        ui->Max_class_value->setValue(0);
        edit();
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_IV_clear3_clicked()
{
    switch(curr_info.template_)
    {
    case 1:
        if(curr_info.max_loop_t == 0) return;
        ui->Max_loop_time->setValue(0);
        edit();
        break;
    default:
        break;
    }
}

void MainWindow::NewFeatures()
{
    on_actionNew_Features_triggered();
}

void MainWindow::on_actionClose_triggered()
{
    if(save_or_not) on_actionSave_S_triggered();
    setWindowTitle("Fractal Designer - Unsaved Project");
    Open_Location = "";
    Info_Save new_info;
    current_info_v = 0;
    redo_max_depth = 1;
    NO_EDIT = true;
    on_actionReset_Colour_Definition_triggered();
    preview_setting = new Preview_Setting(this);
    NO_EDIT = false;
    Project_Template = "Undefined";
    curr_info = new_info;
    save_or_not = false;
    display();

    ui->actionClose->setDisabled(true);
}

void MainWindow::on_commandLinkButton_3_clicked()
{
    on_actionCreate_Video_triggered();
}

void MainWindow::on_actionCreate_Images_in_Range_triggered()
{
    if(!ui->actionCreate_Images_in_Range->isEnabled()) return;
    /*
    if(User_Name.isEmpty())
    {
        QMessageBox::warning(this, "Can not create images", "You have not logged in.");
        return;
    }*/

    if(!save_or_not)
    {
        QMessageBox::warning(this, "Can not create images", "You have not saved the project.");
        return;
    }

    if(Open_Location == "")
    {
        QMessageBox::warning(this, "Can not create images", "You have not chosen a template.");
        return;
    }

    if(!isRouteValid)
    {
        QMessageBox::warning(this, "Can not create images", "The Route Settings are invalid.");
        return;
    }

    int limit = ui->comboBox_fps->currentText().toInt() * (ui->timeEdit->time().second() + 60 * ui->timeEdit->time().minute());
    Create_Images_Range* dialog = new Create_Images_Range(this, limit);
    dialog->show();
    dialog->exec();
    if(to_i == -1) return;

    ui->actionStop->setDisabled(false);
    ui->actionCreate_Images->setDisabled(true);
    ui->actionCreate_Images_in_Range->setDisabled(true);
    ui->actionCheck_Images->setDisabled(true);

    qDebug() << ui->timeEdit->time().second() + 60 * ui->timeEdit->time().minute();
    int total_image = ui->comboBox_fps->currentText().toInt() * (ui->timeEdit->time().second() + 60 * ui->timeEdit->time().minute());
    int current_index = 0;
    int X = ui->Image_size_X->value();
    int Y = ui->Image_size_Y->value();
    QString image_format = "png";
    QString path = ui->lineEdit_imagePath->text();
    QString name = ui->lineEdit_imagePrefix->text();

    if(path.isEmpty() || !QDir(path).exists())
    {
        QMessageBox::warning(this, "Can not create images", "The path does not exist.");
        return;
    }

    create_image_info = new Create_Image_Info();
    connect(this, &MainWindow::build_image_info_signal, create_image_info, &Create_Image_Info::set_info);
    connect(this, &MainWindow::build_image_updateInfo_signal, create_image_info, &Create_Image_Info::updateInfo);
    //connect(create_image_info, &Create_Image_Info::close, create_image_info, &Create_Image_Info::close_create_image_info);
    create_image_info->show();
    emit build_image_info_signal(path + "\\" + name, image_format, to_i - from_i + 1, from_i);
    qDebug() << from_i << to_i;

    // New version with the normal speed
    for(int i = 0; i <= total_image - 1; i++)
    {
        double T = static_cast<double>(i) / total_image;
        while(current_index + 2 < table_route_model->rowCount() && T > Tb(current_index + 1, 0))
        {
            current_index++;
        }
        if(i < from_i || i > to_i) continue;

        double w1 = Tb(current_index, 4);
        double w2 = Tb(current_index + 1, 4);
        double t1 = Tb(current_index, 0);
        double t2 = Tb(current_index + 1, 0);
        double x1 = Tb(current_index, 1);
        double x2 = Tb(current_index + 1, 1);
        double y1 = Tb(current_index, 2);
        double y2 = Tb(current_index + 1, 2);
        double k  = Tb(current_index, 5);
        double t  = (T - t1) / (t2 - t1);
        double x, y;

        double angle = Tb(current_index, 3)
                + (Tb(current_index + 1, 3) - Tb(current_index, 3))
                * ((1 - k) * t + k * pow(t, 2));
        double width = 1 / (t * (1 / w2) + (1 - t) * (1 / w1));

        if(fabs(1 - w1 / w2) < 1E-5)
        {
            x = x1 + t * x2 + (1 - t) * x1;
            y = y1 + t * y2 + (1 - t) * y1;
        }
        else
        {
            x = x1 + (x2 - x1) / log(w1 / w2) * log((w1 / w2 - 1) * t + 1);
            y = y1 + (y2 - y1) / log(w1 / w2) * log((w1 / w2 - 1) * t + 1);
        }

        qDebug() << t << current_index << x << y << width << angle;

        Create_Image_Task* create_images = new Create_Image_Task(this);
        if (!createImagePre(create_images))
        {
            delete create_images;
            return;
        }
        if(curr_info.template_ == 2)
        {
            std::complex<double> c1 = curr_info.Julia_c1, c2 = curr_info.Julia_c2;
            double k = curr_info.Julia_c_rate;
            create_images->setTemplate2(c1 + (c2 - c1) * std::complex<double>((1 - k) * T + k * T * T));
        }
        if(curr_info.template_ == 4)
        {
            double& k = curr_info.Newton_c_rate;

            std::complex<double> arr[10];
            for(int i = 0; i != 10; i++)
            {
                arr[i] = _curr_complex(curr_info.Newton_xn_1[i], curr_info.Newton_xn_2[i], T, k);
            }

            create_images->setTemplate4(_curr_complex(curr_info.Newton_a_1, curr_info.Newton_a_2, T, k),
                                  arr,
                                  _curr_complex(curr_info.Newton_sin_1, curr_info.Newton_sin_2, T, k),
                                  _curr_complex(curr_info.Newton_cos_1, curr_info.Newton_cos_2, T, k),
                                  _curr_complex(curr_info.Newton_ex_1, curr_info.Newton_ex_2, T, k));
        }
        if(i != to_i)
        {
            create_images->setImage(x, y, width, width * Y / X, X, Y, angle, T, image_format, path, name + QString::number(i), "Create_Image", curr_info.y_inverse);
        }
        else
        {
            create_images->setImage(x, y, width, width * Y / X, X, Y, angle, T, image_format, path, name + QString::number(i), "Create_Image_Last", curr_info.y_inverse);
        }
        QThreadPool::globalInstance()->start(create_images);
    }

    from_i = 0;
    to_i = -1;
}

void MainWindow::createImagesInList(const QList<int>& list)
{
    ui->actionStop->setDisabled(false);
    ui->actionCreate_Images->setDisabled(true);
    ui->actionCreate_Images_in_Range->setDisabled(true);

    qDebug() << ui->timeEdit->time().second() + 60 * ui->timeEdit->time().minute();
    int total_image = ui->comboBox_fps->currentText().toInt() * (ui->timeEdit->time().second() + 60 * ui->timeEdit->time().minute());
    int current_index = 0;
    int X = ui->Image_size_X->value();
    int Y = ui->Image_size_Y->value();
    QString image_format = "png";
    QString path = ui->lineEdit_imagePath->text();
    QString name = ui->lineEdit_imagePrefix->text();

    if(path.isEmpty() || !QDir(path).exists())
    {
        QMessageBox::warning(this, "Can not create images", "The path does not exist.");
        return;
    }

    create_image_info = new Create_Image_Info();
    connect(this, &MainWindow::build_image_info_signal_, create_image_info, &Create_Image_Info::set_info_);
    connect(this, &MainWindow::build_image_updateInfo_signal, create_image_info, &Create_Image_Info::updateInfo);
    //connect(create_image_info, &Create_Image_Info::close, create_image_info, &Create_Image_Info::close_create_image_info);
    create_image_info->show();
    emit build_image_info_signal_(path + "\\" + name, image_format, list);
    qDebug() << from_i << to_i;

    // New version with the normal speed
    for(int i = 0; i <= total_image - 1; i++)
    {
        double T = static_cast<double>(i) / total_image;
        while(current_index + 2 < table_route_model->rowCount() && T > Tb(current_index + 1, 0))
        {
            current_index++;
        }
        if(!list.contains(i)) continue;

        double w1 = Tb(current_index, 4);
        double w2 = Tb(current_index + 1, 4);
        double t1 = Tb(current_index, 0);
        double t2 = Tb(current_index + 1, 0);
        double x1 = Tb(current_index, 1);
        double x2 = Tb(current_index + 1, 1);
        double y1 = Tb(current_index, 2);
        double y2 = Tb(current_index + 1, 2);
        double k  = Tb(current_index, 5);
        double t  = (T - t1) / (t2 - t1);
        double x, y;

        double angle = Tb(current_index, 3)
                + (Tb(current_index + 1, 3) - Tb(current_index, 3))
                * ((1 - k) * t + k * pow(t, 2));
        double width = 1 / (t * (1 / w2) + (1 - t) * (1 / w1));

        if(fabs(1 - w1 / w2) < 1E-5)
        {
            x = x1 + t * x2 + (1 - t) * x1;
            y = y1 + t * y2 + (1 - t) * y1;
        }
        else
        {
            x = x1 + (x2 - x1) / log(w1 / w2) * log((w1 / w2 - 1) * t + 1);
            y = y1 + (y2 - y1) / log(w1 / w2) * log((w1 / w2 - 1) * t + 1);
        }

        Create_Image_Task* create_images = new Create_Image_Task(this);
        if (!createImagePre(create_images))
        {
            delete create_images;
            return;
        }

        if(curr_info.template_ == 2)
        {
            std::complex<double> c1 = curr_info.Julia_c1, c2 = curr_info.Julia_c2;
            double k = curr_info.Julia_c_rate;
            create_images->setTemplate2(c1 + (c2 - c1) * std::complex<double>((1 - k) * T + k * T * T));
        }
        if(curr_info.template_ == 4)
        {
            double& k = curr_info.Newton_c_rate;

            std::complex<double> arr[10];
            for(int i = 0; i != 10; i++)
            {
                arr[i] = _curr_complex(curr_info.Newton_xn_1[i], curr_info.Newton_xn_2[i], T, k);
            }

            create_images->setTemplate4(_curr_complex(curr_info.Newton_a_1, curr_info.Newton_a_2, T, k),
                                  arr,
                                  _curr_complex(curr_info.Newton_sin_1, curr_info.Newton_sin_2, T, k),
                                  _curr_complex(curr_info.Newton_cos_1, curr_info.Newton_cos_2, T, k),
                                  _curr_complex(curr_info.Newton_ex_1, curr_info.Newton_ex_2, T, k));
        }

        // qDebug() << t << current_index << x << y << width << angle;
        if(i != *(--list.end()))
        {
            create_images->setImage(x, y, width, width * Y / X, X, Y, angle, T, image_format, path, name + QString::number(i), "Create_Image", curr_info.y_inverse);
        }
        else
        {
            create_images->setImage(x, y, width, width * Y / X, X, Y, angle, T, image_format, path, name + QString::number(i), "Create_Image_Last", curr_info.y_inverse);
        }
        QThreadPool::globalInstance()->start(create_images);
    }
}

void MainWindow::on_actionVersion_3_triggered()
{
    QDesktopServices::openUrl(QUrl("https://www.bilibili.com/video/BV17K4y1J7XM"));
}

void MainWindow::on_actionVersion_4_triggered()
{
    QDesktopServices::openUrl(QUrl("https://www.bilibili.com/video/BV1wB4y1N7pU"));
}

void MainWindow::on_actionGitHub_Repository_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/Teddy-van-Jerry/Fractal_Designer"));
}

void MainWindow::on_actionCheck_Images_triggered()
{
    if(!ui->actionCheck_Images->isEnabled()) return;

    int total_image = ui->comboBox_fps->currentText().toInt() * (ui->timeEdit->time().second() + 60 * ui->timeEdit->time().minute());
    QList<int> Missed_Images;
    for(int i = 0; i != total_image; i++)
    {
        if(!QFile(ui->lineEdit_imagePath->text() + "/" + ui->lineEdit_imagePrefix->text() + QString::number(i) + ".png").exists())
        {
            Missed_Images.push_back(i);
        }
        // else
        // {
        //     if(QImage(ui->lineEdit_imagePath->text() + "/" + ui->lineEdit_imagePrefix->text() + QString::number(i) + ".png").isNull())
        //     {
        //         Missed_Images.push_back(i);
        //     }
        // }
    }
    QString content;
    if(Missed_Images.size() == 0)
    {
        content = "No missing images!";
        QMessageBox::information(this, "Check Finished", content, QMessageBox::Ok);
    }
    else if(Missed_Images.size() < 10)
    {
        content = "Missing images with the number:\n";
        content.append(QString::number(Missed_Images[0]));
        for(int i = 1; i != Missed_Images.size(); i++)
        {
            content.append(tr(", ") + QString::number(Missed_Images[i]));
        }
        content.append(".\nDo you want to recreate them?");
        if(QMessageBox::information(this, "Check Finished", content, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if(!ui->actionCreate_Images_in_Range->isEnabled()) return;
            if(User_Name.isEmpty())
            {
                QMessageBox::warning(this, "Can not create images", "You have not logged in.");
                return;
            }

            if(!save_or_not)
            {
                QMessageBox::warning(this, "Can not create images", "You have not saved the project.");
                return;
            }

            if(Open_Location == "")
            {
                QMessageBox::warning(this, "Can not create images", "You have not chosen a template.");
                return;
            }

            if(!isRouteValid)
            {
                QMessageBox::warning(this, "Can not create images", "The Route Settings are invalid.");
                return;
            }
            createImagesInList(Missed_Images);
        }
    }
    else
    {
        content = "Missing images with the number:\n";
        content.append(QString::number(Missed_Images[0]));
        for(int i = 1; i != 10; i++)
        {
            content.append(tr(", ") + QString::number(Missed_Images[i]));
        }
        content.append(", ..., " + QString::number(*(--Missed_Images.end())) + ".\nDo you want to recreate them?");
        if(QMessageBox::information(this, "Check Finished", content, QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if(!ui->actionCreate_Images_in_Range->isEnabled()) return;
            if(User_Name.isEmpty())
            {
                QMessageBox::warning(this, "Can not create images", "You have not logged in.");
                return;
            }

            if(!save_or_not)
            {
                QMessageBox::warning(this, "Can not create images", "You have not saved the project.");
                return;
            }

            if(Open_Location == "")
            {
                QMessageBox::warning(this, "Can not create images", "You have not chosen a template.");
                return;
            }

            if(!isRouteValid)
            {
                QMessageBox::warning(this, "Can not create images", "The Route Settings are invalid.");
                return;
            }
            createImagesInList(Missed_Images);
        }
    }
}

bool MainWindow::existImage(int i) const
{
    QFile image_file(ui->lineEdit_imagePath->text() + "/" + ui->lineEdit_imagePrefix->text() + QString::number(i) + ".png");
    if(image_file.exists()) return true;
    else return false;
}

void MainWindow::deleteImage(int i)
{
    QFile image_file(ui->lineEdit_imagePath->text() + "/" + ui->lineEdit_imagePrefix->text() + QString::number(i) + ".png");
    image_file.remove();
}

void MainWindow::on_actionDelete_Images_triggered()
{
    int total_image = ui->comboBox_fps->currentText().toInt() * (ui->timeEdit->time().second() + 60 * ui->timeEdit->time().minute());
    for(int i = 0; i != total_image; i++)
    {
        deleteImage(i);
    }
    int skip_number = 0;
    int check_till = total_image - 1;
    while(skip_number++ < 10000)
    {
        if(existImage(++check_till))
        {
            deleteImage(check_till);
            skip_number = 0;
        }
    }
    QMessageBox::information(this, "Information", "Deleting Images Finished!");
}

void MainWindow::on_actionMost_triggered()
{
    ui->actionMost->setChecked(true);
    ui->actionMore->setChecked(false);
    ui->actionNormal->setChecked(false);
    ui->actionLess->setChecked(false);
    ui->actionLeast->setChecked(false);
    QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount());
}

void MainWindow::on_actionMore_triggered()
{
    ui->actionMost->setChecked(false);
    ui->actionMore->setChecked(true);
    ui->actionNormal->setChecked(false);
    ui->actionLess->setChecked(false);
    ui->actionLeast->setChecked(false);
    int total_thread = QThread::idealThreadCount();
    total_thread = total_thread * 3 / 4 < 2 ? 2 : total_thread * 3 / 4;
    QThreadPool::globalInstance()->setMaxThreadCount(total_thread);
}

void MainWindow::on_actionNormal_triggered()
{
    ui->actionMost->setChecked(false);
    ui->actionMore->setChecked(false);
    ui->actionNormal->setChecked(true);
    ui->actionLess->setChecked(false);
    ui->actionLeast->setChecked(false);
    int total_thread = QThread::idealThreadCount();
    total_thread = total_thread * 1 / 2 < 2 ? 2 : total_thread * 1 / 2;
    QThreadPool::globalInstance()->setMaxThreadCount(total_thread);
}

void MainWindow::on_actionLess_triggered()
{
    ui->actionMost->setChecked(false);
    ui->actionMore->setChecked(false);
    ui->actionNormal->setChecked(false);
    ui->actionLess->setChecked(true);
    ui->actionLeast->setChecked(false);
    int total_thread = QThread::idealThreadCount();
    total_thread = total_thread * 1 / 3 < 2 ? 2 : total_thread / 3;
    QThreadPool::globalInstance()->setMaxThreadCount(total_thread);
}

void MainWindow::on_actionLeast_triggered()
{
    ui->actionMost->setChecked(false);
    ui->actionMore->setChecked(false);
    ui->actionNormal->setChecked(false);
    ui->actionLess->setChecked(false);
    ui->actionLeast->setChecked(true);
    int total_thread = QThread::idealThreadCount();
    total_thread = total_thread == 1 ? 1 : 2;
    QThreadPool::globalInstance()->setMaxThreadCount(total_thread);
}

void MainWindow::on_actionTemplate_2_triggered()
{
    template_2_dialog->setGeometry(x() + width() / 2 - 15, y() + 130, width() / 2, 342);
    template_2_dialog->show();
}

void MainWindow::on_actionTemplate_6_triggered() // Template 4 in Menu Additional Template Settings
{
    template_4_dialog->setGeometry(x() + width() / 2 - 15, y() + 130, width() / 2, 493);
    template_4_dialog->show();
}

void MainWindow::on_actionVersion_5_triggered() // Version 1 of Sample Video Template 3
{
    QDesktopServices::openUrl(QUrl("https://www.bilibili.com/video/BV1RA41157kJ"));
}

void MainWindow::on_actionVersion_6_triggered() // Version 1 of Sample Video Template 4
{
    QDesktopServices::openUrl(QUrl("https://www.bilibili.com/video/BV1Tb4y1D7oN"));
}

void MainWindow::on_MainWindow_AboutFD_clicked()
{
    QDesktopServices::openUrl(QUrl("https://frd.teddy-van-jerry.org/about/"));
}

void MainWindow::on_actionChinese_2_triggered() // language Chinese
{
    if (app_language == LANGUAGE_ENGLISH)
    {
        app_language = LANGUAGE_CHINESE;
        translator = new QTranslator(qApp);
        if (translator->load(":/Languages/FRD_zh_CN.qm"))
        {
            qApp->installTranslator(translator);
            ui->retranslateUi(this);
            QSettings setting(QCoreApplication::applicationDirPath() + "/Language.ini", QSettings::IniFormat);
            setting.beginGroup("LANGUAGE");
            setting.setValue("Language", "CHINESE");
            setting.endGroup();
            QMessageBox::information(this, "提示", "语言已切换至中文，\n重启软件可以看到语言改变。");

        }
        else
        {
            qDebug() << "Fail to switch to Chinese";
            return;
        }
    }
    else
    {
        return;
    }
    ui->actionEnglish_2->setChecked(false);
}

void MainWindow::on_actionEnglish_2_triggered() // language English
{
    if (language_setting_no_change_now) return;
    language_setting_no_change_now = true;
    if (app_language == LANGUAGE_CHINESE)
    {
        app_language = LANGUAGE_ENGLISH;
        translator = new QTranslator(qApp);
        if (translator->load(":/Languages/FRD_en_UK.qm"))
        {
            qApp->installTranslator(translator);
            ui->retranslateUi(this);
            QSettings setting(QCoreApplication::applicationDirPath() + "/Language.ini", QSettings::IniFormat);
            setting.beginGroup("LANGUAGE");
            setting.setValue("Language", "ENGLISH");
            setting.endGroup();
            QMessageBox::information(this, "Information", "The language has been switched into English.\nRestart the programme to change the language.");
        }
        else
        {
            qDebug() << "Fail to switch to English";
            return;
        }
    }
    else
    {
        return;
    }
    ui->actionChinese_2->setChecked(false);
    language_setting_no_change_now = false;
}

void MainWindow::setLanguage(App_Language la)
{
    if (language_setting_no_change_now) return;
    language_setting_no_change_now = true;
    if(la == LANGUAGE_CHINESE)
    {
        app_language = LANGUAGE_CHINESE;
        ui->actionEnglish_2->setChecked(false);
        ui->actionChinese_2->setChecked(true);
    }
    else
    {
        app_language = LANGUAGE_ENGLISH;
        ui->actionEnglish_2->setChecked(true);
        ui->actionChinese_2->setChecked(false);
    }
    language_setting_no_change_now = false;
}

void MainWindow::on_Convergent_Points_Colour_Formula_textChanged()
{
    save_or_not = false;
}

void MainWindow::on_Divergent_Points_Colour_Formula_textChanged()
{
    save_or_not = false;
}

void MainWindow::on_lineEdit_Custom_Formula_editingFinished()
{
    if (ui->lineEdit_Custom_Formula->text() != curr_info.CustomFormula_)
    {
        edit();
    }
}

void MainWindow::on_actionPreview_Settings_triggered()
{
    preview_setting->show();
}

void MainWindow::on_actionReset_Colour_Definition_triggered()
{
    QString str = "R = 0;\nG = 0;\nB = 0;\nA = 255;";
    bool edit_here = false;
    if (ui->Convergent_Points_Colour_Formula->toPlainText() != str)
    {
        ui->Convergent_Points_Colour_Formula->setPlainText(str);
        edit_here = true;
    }
    if (ui->Divergent_Points_Colour_Formula->toPlainText() != str)
    {
        ui->Divergent_Points_Colour_Formula->setPlainText(str);
        edit_here = true;
    }
    if (edit_here)
    {
        edit();
    }
}

void MainWindow::on_checkBox_yInverse_stateChanged(int arg1)
{
    buff_info.y_inverse = arg1;
    edit(EDIT_ALREADY);
}

void MainWindow::on_pushButton_Template_Help_clicked()
{
    QDesktopServices::openUrl(QUrl("https://frd.teddy-van-jerry.org/sample/fractal-designer-5-6-lts-sample-1/"));
}

void MainWindow::useDarkIcon()
{
    isDarkStyle=true;
    ui->actionCheck_Images->setIcon(QIcon(":/icon/Menu Icon/dark/Check Images.svg"));
    ui->actionClose->setIcon(QIcon(":/icon/Menu Icon/dark/Close.svg"));
    ui->actionCreate_Images->setIcon(QIcon(":/icon/Menu Icon/dark/Create Image.svg"));
    ui->actionCreate_Images_in_Range->setIcon(QIcon(":/icon/Menu Icon/dark/Create Images in Range.svg"));
    ui->actionCreate_Video->setIcon(QIcon(":/icon/Menu Icon/dark/Create Video.svg"));
    ui->actionDelete->setIcon(QIcon(":/icon/Menu Icon/dark/Delete.svg"));
    ui->actionDelete_Images->setIcon(QIcon(":/icon/Menu Icon/dark/DeleteImage.svg"));
    ui->actionDelete_All_Parameters->setIcon(QIcon(":/icon/Menu Icon/dark/Delete.svg"));
    ui->actionExit_E->setIcon(QIcon(":/icon/Menu Icon/dark/Exit.svg"));
    ui->actionStop->setIcon(QIcon(":/icon/Menu Icon/dark/Stop.svg"));
    ui->actionUndo->setIcon(QIcon(":/icon/Menu Icon/dark/Undo.svg"));
    ui->actionRoute_Tool->setIcon(QIcon(":/icon/Menu Icon/dark/Route_Tool.svg"));
    ui->actionSave_S->setIcon(QIcon(":/icon/Menu Icon/dark/Save.svg"));
    ui->actionSave_As_A->setIcon(QIcon(":/icon/Menu Icon/dark/SaveAs.svg"));
    ui->actionOpen_O->setIcon(QIcon(":/icon/Menu Icon/dark/Open.svg"));
    ui->actionVersion->setIcon(QIcon(":/icon/Menu Icon/dark/Version.png"));
    ui->menuPreview->setIcon(QIcon(":/icon/Menu Icon/dark/Preview.png"));
    ui->actionGitHub_Repository->setIcon(QIcon(":/icon/Menu Icon/dark/GitHub.png"));
    ui->actionRedo->setIcon(QIcon(":/icon/Menu Icon/dark/Redo.svg"));
    ui->actionNew_N->setIcon(QIcon(":/icon/Menu Icon/dark/New.svg"));

    ui->actionPreview_Refresh->setIcon(QIcon(":/icon/Menu Icon/dark/Refresh.svg"));
    Button_Search->setIcon(QIcon(":/icon/Menu Icon/dark/Search.svg"));
    ui->menuWizard->setIcon(QIcon(":/icon/Menu Icon/dark/Wizard.svg"));
    // ui->action->setIcon(QIcon(":/icon/Menu Icon/dark/Previous.svg"));
    ui->toolButton_imagePath->setIcon(QIcon(":/icon/Menu Icon/dark/FolderBrowser.svg"));
    ui->toolButton_videoPath->setIcon(QIcon(":/icon/Menu Icon/dark/FolderBrowser.svg"));
    ui->actionEnglish->setIcon(QIcon(":/icon/Menu Icon/dark/Help.svg"));
    // ui->menuPicture_P->setIcon(QIcon(":/icon/Menu Icon/dark/Image.svg"));
    // ui->action->setIcon(QIcon(":/icon/Menu Icon/dark/Next.svg"));

    this->m_titleBar->btn_close.setIcon(QIcon(":/EXE Icons/Close_White.svg"));
    this->m_titleBar->btn_maximize.setIcon(QIcon(":/EXE Icons/Maximize_white.svg"));
    this->m_titleBar->btn_minimize.setIcon(QIcon(":/EXE Icons/Minimize_white.svg"));
}

void MainWindow::useWhiteIcon()
{
    isDarkStyle=false;
    ui->actionCheck_Images->setIcon(QIcon(":/icon/Menu Icon/Check Images.svg"));
    ui->actionClose->setIcon(QIcon(":/icon/Menu Icon/Close.svg"));
    ui->actionCreate_Images->setIcon(QIcon(":/icon/Menu Icon/Create Image.svg"));
    ui->actionCreate_Images_in_Range->setIcon(QIcon(":/icon/Menu Icon/Create Images in Range.svg"));
    ui->actionCreate_Video->setIcon(QIcon(":/icon/Menu Icon/Create Video.svg"));
    ui->actionDelete->setIcon(QIcon(":/icon/Menu Icon/Delete.svg"));
    ui->actionDelete_Images->setIcon(QIcon(":/icon/Menu Icon/DeleteImage.svg"));
    ui->actionDelete_All_Parameters->setIcon(QIcon(":/icon/Menu Icon/Delete.svg"));
    ui->actionExit_E->setIcon(QIcon(":/icon/Menu Icon/Exit.svg"));
    ui->actionStop->setIcon(QIcon(":/icon/Menu Icon/Stop.svg"));
    ui->actionUndo->setIcon(QIcon(":/icon/Menu Icon/Undo.svg"));
    ui->actionRoute_Tool->setIcon(QIcon(":/icon/Menu Icon/Route_Tool.svg"));
    ui->actionSave_S->setIcon(QIcon(":/icon/Menu Icon/Save.svg"));
    ui->actionSave_As_A->setIcon(QIcon(":/icon/Menu Icon/SaveAs.svg"));
    ui->actionOpen_O->setIcon(QIcon(":/icon/Menu Icon/Open.svg"));
    ui->actionVersion->setIcon(QIcon(":/icon/Menu Icon/Version.png"));
    ui->menuPreview->setIcon(QIcon(":/icon/Menu Icon/Preview.png"));
    ui->actionGitHub_Repository->setIcon(QIcon(":/icon/Menu Icon/GitHub.png"));
    ui->actionRedo->setIcon(QIcon(":/icon/Menu Icon/Redo.svg"));
    ui->actionNew_N->setIcon(QIcon(":/icon/Menu Icon/New.svg"));

    ui->actionPreview_Refresh->setIcon(QIcon(":/icon/Menu Icon/Refresh.svg"));
    Button_Search->setIcon(QIcon(":/icon/Menu Icon/Search.svg"));
    ui->menuWizard->setIcon(QIcon(":/icon/Menu Icon/Wizard.svg"));
    // ui->action->setIcon(QIcon(":/icon/Menu Icon/Previous.svg"));
    ui->toolButton_imagePath->setIcon(QIcon(":/icon/Menu Icon/FolderBrowser.svg"));
    ui->toolButton_videoPath->setIcon(QIcon(":/icon/Menu Icon/FolderBrowser.svg"));
    ui->actionEnglish->setIcon(QIcon(":/icon/Menu Icon/Help.svg"));
    // ui->menuPicture_P->setIcon(QIcon(":/icon/Menu Icon/Image.svg"));
    // ui->action->setIcon(QIcon(":/icon/Menu Icon/Next.svg"));

    this->m_titleBar->btn_close.setIcon(QIcon(":/EXE Icons/Close.svg"));
    this->m_titleBar->btn_maximize.setIcon(QIcon(":/EXE Icons/Maximize.svg"));
    this->m_titleBar->btn_minimize.setIcon(QIcon(":/EXE Icons/Minimize.svg"));
}

void MainWindow::on_actionTheme_Light_triggered()
{
    qDebug() << "here";
    useWhiteIcon();
    ui->actionTheme_Light->setChecked(true);
    ui->actionTheme_Amoled->setChecked(false);
    ui->actionTheme_Aqua->setChecked(false);
    ui->actionTheme_Console->setChecked(false);
    ui->actionTheme_Elegant->setChecked(false);
    ui->actionTheme_Macos->setChecked(false);
    ui->actionTheme_ManjaroMix->setChecked(false);
    ui->actionTheme_MaterialDark->setChecked(false);
    ui->actionTheme_Ubuntu->setChecked(false);
    QFile styleFile( ":/StyleSheet/Default.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    setStyleSheet( style );
    WriteInit("StyleSheet", "actionTheme_Light");
    updateMaxButton();
}

void MainWindow::on_actionTheme_Amoled_triggered()
{
    useDarkIcon();
    ui->actionTheme_Light->setChecked(false);
    ui->actionTheme_Amoled->setChecked(true);
    ui->actionTheme_Aqua->setChecked(false);
    ui->actionTheme_Console->setChecked(false);
    ui->actionTheme_Elegant->setChecked(false);
    ui->actionTheme_Macos->setChecked(false);
    ui->actionTheme_ManjaroMix->setChecked(false);
    ui->actionTheme_MaterialDark->setChecked(false);
    ui->actionTheme_Ubuntu->setChecked(false);
    QFile styleFile( ":/StyleSheet/AMOLED.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    setStyleSheet( style );
    WriteInit("StyleSheet", "actionTheme_Amoled");
    updateMaxButton();
}


void MainWindow::on_actionTheme_Aqua_triggered()
{
    useWhiteIcon();
    ui->actionTheme_Light->setChecked(false);
    ui->actionTheme_Amoled->setChecked(false);
    ui->actionTheme_Aqua->setChecked(true);
    ui->actionTheme_Console->setChecked(false);
    ui->actionTheme_Elegant->setChecked(false);
    ui->actionTheme_Macos->setChecked(false);
    ui->actionTheme_ManjaroMix->setChecked(false);
    ui->actionTheme_MaterialDark->setChecked(false);
    ui->actionTheme_Ubuntu->setChecked(false);
    QFile styleFile( ":/StyleSheet/Aqua.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    setStyleSheet( style );
    WriteInit("StyleSheet", "actionTheme_Aqua");
    updateMaxButton();
}


void MainWindow::on_actionTheme_Console_triggered()
{
    useDarkIcon();
    ui->actionTheme_Light->setChecked(false);
    ui->actionTheme_Amoled->setChecked(false);
    ui->actionTheme_Aqua->setChecked(false);
    ui->actionTheme_Console->setChecked(true);
    ui->actionTheme_Elegant->setChecked(false);
    ui->actionTheme_Macos->setChecked(false);
    ui->actionTheme_ManjaroMix->setChecked(false);
    ui->actionTheme_MaterialDark->setChecked(false);
    ui->actionTheme_Ubuntu->setChecked(false);
    QFile styleFile( ":/StyleSheet/ConsoleStyle.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    setStyleSheet( style );
    WriteInit("StyleSheet", "actionTheme_Console");
    updateMaxButton();
}


void MainWindow::on_actionTheme_Elegant_triggered()
{
    useDarkIcon();
    ui->actionTheme_Light->setChecked(false);
    ui->actionTheme_Amoled->setChecked(false);
    ui->actionTheme_Aqua->setChecked(false);
    ui->actionTheme_Console->setChecked(false);
    ui->actionTheme_Elegant->setChecked(true);
    ui->actionTheme_Macos->setChecked(false);
    ui->actionTheme_ManjaroMix->setChecked(false);
    ui->actionTheme_MaterialDark->setChecked(false);
    ui->actionTheme_Ubuntu->setChecked(false);
    QFile styleFile( ":/StyleSheet/ElegantDark.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    setStyleSheet( style );
    WriteInit("StyleSheet", "actionTheme_Elegant");
    updateMaxButton();
}


void MainWindow::on_actionTheme_Macos_triggered()
{
    useWhiteIcon();
    ui->actionTheme_Light->setChecked(false);
    ui->actionTheme_Amoled->setChecked(false);
    ui->actionTheme_Aqua->setChecked(false);
    ui->actionTheme_Console->setChecked(false);
    ui->actionTheme_Elegant->setChecked(false);
    ui->actionTheme_Macos->setChecked(true);
    ui->actionTheme_ManjaroMix->setChecked(false);
    ui->actionTheme_MaterialDark->setChecked(false);
    ui->actionTheme_Ubuntu->setChecked(false);
    QFile styleFile( ":/StyleSheet/MacOS.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    setStyleSheet( style );
    WriteInit("StyleSheet", "actionTheme_Macos");
    updateMaxButton();
}


void MainWindow::on_actionTheme_ManjaroMix_triggered()
{
    useDarkIcon();
    ui->actionTheme_Light->setChecked(false);
    ui->actionTheme_Amoled->setChecked(false);
    ui->actionTheme_Aqua->setChecked(false);
    ui->actionTheme_Console->setChecked(false);
    ui->actionTheme_Elegant->setChecked(false);
    ui->actionTheme_Macos->setChecked(false);
    ui->actionTheme_ManjaroMix->setChecked(true);
    ui->actionTheme_MaterialDark->setChecked(false);
    ui->actionTheme_Ubuntu->setChecked(false);
    QFile styleFile( ":/StyleSheet/ManjaroMix.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    setStyleSheet( style );
    WriteInit("StyleSheet", "actionTheme_ManjaroMix");
    updateMaxButton();
}


void MainWindow::on_actionTheme_MaterialDark_triggered()
{
    useDarkIcon();
    ui->actionTheme_Light->setChecked(false);
    ui->actionTheme_Amoled->setChecked(false);
    ui->actionTheme_Aqua->setChecked(false);
    ui->actionTheme_Console->setChecked(false);
    ui->actionTheme_Elegant->setChecked(false);
    ui->actionTheme_Macos->setChecked(false);
    ui->actionTheme_ManjaroMix->setChecked(false);
    ui->actionTheme_MaterialDark->setChecked(true);
    ui->actionTheme_Ubuntu->setChecked(false);
    QFile styleFile( ":/StyleSheet/MaterialDark.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    setStyleSheet( style );
    WriteInit("StyleSheet", "actionTheme_MaterialDark");
    updateMaxButton();
}


void MainWindow::on_actionTheme_Ubuntu_triggered()
{
    useWhiteIcon();
    ui->actionTheme_Light->setChecked(false);
    ui->actionTheme_Amoled->setChecked(false);
    ui->actionTheme_Aqua->setChecked(false);
    ui->actionTheme_Console->setChecked(false);
    ui->actionTheme_Elegant->setChecked(false);
    ui->actionTheme_Macos->setChecked(false);
    ui->actionTheme_ManjaroMix->setChecked(false);
    ui->actionTheme_MaterialDark->setChecked(false);
    ui->actionTheme_Ubuntu->setChecked(true);
    QFile styleFile( ":/StyleSheet/Ubuntu.qss" );
    styleFile.open( QFile::ReadOnly );
    QString style( styleFile.readAll() );
    setStyleSheet( style );
    WriteInit("StyleSheet", "actionTheme_Ubuntu");
    updateMaxButton();
}

QString MainWindow::ReadInit(const QString& key)
{
    QString path = QApplication::applicationDirPath() + "/Config.ini";

    QSettings config(path, QSettings::IniFormat);
    QVariant variant = config.value(QString("config/") + key);
    return variant.value<QString>();
}

void MainWindow::WriteInit(const QString& key, const QString& value)
{
    QString path = QApplication::applicationDirPath() + "/Config.ini";
    QSettings config(path, QSettings::IniFormat);

    QVariant variant;
    variant.setValue(value);
    config.beginGroup("config");
    config.setValue(key, variant);
    config.endGroup();
}

void MainWindow::ReadStyle()
{
    QString style = ReadInit("StyleSheet");
    if(style=="actionTheme_Light")
    {
        on_actionTheme_Light_triggered();
    }
    else if(style=="actionTheme_Amoled")
    {
        on_actionTheme_Amoled_triggered();
    }
    else if(style=="actionTheme_Aqua")
    {
        on_actionTheme_Aqua_triggered();
    }
    else if(style=="actionTheme_Console")
    {
        on_actionTheme_Console_triggered();
    }
    else if(style=="actionTheme_Elegant")
    {
        on_actionTheme_Elegant_triggered();
    }
    else if(style=="actionTheme_Macos")
    {
        on_actionTheme_Macos_triggered();
    }
    else if(style=="actionTheme_ManjaroMix")
    {
        on_actionTheme_ManjaroMix_triggered();
    }
    else if(style=="actionTheme_MaterialDark")
    {
        on_actionTheme_MaterialDark_triggered();
    }
    else if(style=="actionTheme_Ubuntu")
    {
        on_actionTheme_Ubuntu_triggered();
    }
    else
    {
        // Default
        on_actionTheme_Light_triggered();
    }
}

void MainWindow::updateMaxButton()
{
    if(!isDarkStyle)
    {
        if(this->isMaximized())
        {
            this->FRD_TitleBar().btn_maximize.setIcon(QIcon(":/EXE Icons/Restore.svg"));

        }
        else if(!this->isMaximized())
        {
            this->FRD_TitleBar().btn_maximize.setIcon(QIcon(":/EXE Icons/Maximize.svg"));
        }
    }
    else
    {
        if(this->isMaximized())
        {
            this->FRD_TitleBar().btn_maximize.setIcon(QIcon(":/EXE Icons/Restore_white.svg"));

        }
        else if(!this->isMaximized())
        {
            this->FRD_TitleBar().btn_maximize.setIcon(QIcon(":/EXE Icons/Maximize_white.svg"));
        }
    }
}

void MainWindow::tableRouteCustomMenuRequested(QPoint pos)
{
    table_route_line = ui->tableView_Route->indexAt(pos).row();
    table_route_action[0]->setEnabled(true);
    table_route_action[1]->setEnabled(true);
    table_route_action[2]->setEnabled(true);
    table_route_action[3]->setEnabled(true);
    table_route_action[4]->setEnabled(true);
    if (table_route_line < 0)
    {
        // outside the table
        table_route_action[0]->setDisabled(true);
        table_route_action[1]->setDisabled(true);
        table_route_action[2]->setDisabled(true);
        table_route_action[4]->setDisabled(true);
    }
    else
    {
        if (table_route_line == table_route_model->rowCount() - 1)
        {
            // the last row
            table_route_action[1]->setDisabled(true);
        }
        if (table_route_line == 0)
        {
            // the first row
            table_route_action[0]->setDisabled(true);
        }
    }
    table_route_menu->popup(ui->tableView_Route->viewport()->mapToGlobal(pos));
}

void MainWindow::tableRouteMoveUp()
{
    table_route_model->insertRow(table_route_line - 1, table_route_model->takeRow(table_route_line));
}

void MainWindow::tableRouteMoveDown()
{
    table_route_model->insertRow(table_route_line, table_route_model->takeRow(table_route_line + 1));
}

void MainWindow::tableRouteInsertBefore()
{
    on_pushButton_routeAdd_clicked();
    table_route_model->insertRow(table_route_line, table_route_model->takeRow(table_route_model->rowCount()));
}

void MainWindow::tableRouteInsertAfter()
{
    on_pushButton_routeAdd_clicked();
    table_route_model->insertRow(table_route_line + 1, table_route_model->takeRow(table_route_model->rowCount()));
}

void MainWindow::tableRouteDeleteRow()
{
    table_route_model->removeRow(table_route_line);
}

QToolBar *MainWindow::generateBorder(Qt::ToolBarArea area,
                                        Qt::Orientation orientation){
    QToolBar *border = new QToolBar("___border___");
    border->setStyleSheet(
        "\nQToolBar {\n"
        "   margin : 1px;\n"
        "   border : 0px;\n"
        "   background : transparent;\n"
        "}"
    );

    if (orientation & Qt::Horizontal){
        border->setMinimumHeight(6);
        border->setMaximumHeight(6);
    }
    else {
        border->setMinimumWidth(6);
        border->setMaximumWidth(6);
    }
    border->setMovable(false);
    border->setFloatable(false);
    border->setAllowedAreas(area);
    border->setMouseTracking(true);
    border->installEventFilter(this);

    this->addToolBar(area, border);
    return border;
}

QMenu * MainWindow::createPopupMenu(){
    QMenu *menu = QMainWindow::createPopupMenu();
    QList<QAction *> removal;
    foreach (QAction *a, menu->actions())
        if (a->text() == "___border___") removal.append(a);
    foreach (QAction *a, removal) menu->removeAction(a);
    return menu;
}

void MainWindow::setMenuBar(QMenuBar *menuBar){
    if (this->m_menuBar == menuBar) return;

    if (this->m_menuBar) {
        if (this->m_menuBar != this->m_menuWidget && this->m_menuWidget){
            this->m_menuWidget->setParent(nullptr);
            this->m_menuWidget->deleteLater();
        }



        this->m_menuBar->hide();
        this->m_menuBar->setParent(nullptr);
        this->m_menuBar->deleteLater();
    }

    this->m_menuBar = menuBar;
    this->m_menuWidget = qobject_cast<QWidget *>(menuBar);

    if (menuBar){
        menuBar->setParent(this);
        this->m_titleBarW->layout()->addWidget(menuBar);
    }
}

QMenuBar* MainWindow::menuBar() const{
    return this->m_menuBar;
}

void MainWindow::setMenuWidget(QWidget *widget){
    if (this->m_menuWidget == widget) return;

    widget->setParent(this);

    if (this->m_menuWidget){
        this->m_menuWidget->hide();
        this->m_menuWidget->setParent(nullptr);
        this->m_menuWidget->deleteLater();
    }

    this->m_menuBar = nullptr;
    this->m_menuWidget = widget;

    if (this->m_menuWidget){
        this->m_menuWidget->setParent(this);
        this->m_titleBarW->layout()->addWidget(widget);
    }
}

QWidget * MainWindow::menuWidget() const{
    return this->m_menuWidget;
}

bool MainWindow::eventFilter(QObject*, QEvent *event){
    if (event->type() == QEvent::MouseMove)
        customMouseMoveEvent(static_cast<QMouseEvent*>(event));
    else if (event->type() == QEvent::MouseButtonPress)
        mousePressEvent(static_cast<QMouseEvent*>(event));
    return false;
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (this->FRD_TitleBar().m_frameButtons & QCustomAttrs::Maximize && this->FRD_TitleBar().btn_maximize.isEnabled()
            && event->buttons() & Qt::LeftButton) {
//        this->FRD_TitleBar().mamaximizing = true;
//        emit requestMaximize();
        updateMaxButton();
    }
    QWidget::mouseDoubleClickEvent(event);
}

bool MainWindow::event(QEvent *event){
    if (event->type() == QEvent::ChildRemoved){
        QChildEvent *evt = static_cast<QChildEvent*>(event);
        if (evt->child()->isWidgetType()) evt->child()->removeEventFilter(this);
    }
    else if (event->type() == QEvent::ChildAdded){
        QChildEvent *evt = static_cast<QChildEvent*>(event);
        if (evt->child()->isWidgetType()){
            QWidget *child = qobject_cast<QWidget*>(evt->child());

            child->setMouseTracking(true);
            child->installEventFilter(this);

            if (child->metaObject()->indexOfClassInfo("custom_obj_type") == -1){
                child->setStyleSheet(child->styleSheet() +
                                     "\nQToolBar {\n"
                                     "    margin : 1px;\n"
                                     "    padding: 0px 6px 0px 6px;\n"
                                     "    border: 1px transparent solid;"
                                     "}\n"
                                     "QToolBar:top:first, QToolBar:bottom:first, QToolBar:left:first {\n"
                                     "    margin : 0px 0px 0px 6px;\n"
                                     "}\n"
                                     "QToolBar:top:only-one, QToolBar:bottom:only-one {\n"
                                     "    margin : 0px 6px 0px 6px;\n"
                                     "}\n"
                                     "QToolBar:top:last, QToolBar:bottom:last, QToolBar:right:last {\n"
                                     "    margin : 0px 6px 0px 0px;\n"
                                     "}\n");
            }
        }
    }
    return QMainWindow::event(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if (event->button() & Qt::LeftButton){
        int x = event->x(), y = event->y();
        int bottom = this->height() - RESIZE_LIMIT, right = this->width() - RESIZE_LIMIT;

        QPoint posCursor = event->globalPos();
        Qt::Edges nFlags = {};
        if (x < RESIZE_LIMIT) {
            nFlags |= Qt::LeftEdge;
            posCursor.rx() -= this->x();
        }
        if (y < RESIZE_LIMIT) {
            nFlags |= Qt::TopEdge;
            posCursor.ry() -= this->y();
        }
        if (x > right) {
            nFlags |= Qt::RightEdge;
            posCursor.rx() -= (this->x() + this->width());
        }
        if (y > bottom) {
            nFlags |= Qt::BottomEdge;
            posCursor.ry() -= (this->y() + this->height());
        }
        this->m_lock = nFlags;
        this->m_posCursor = posCursor;
    }
    QMainWindow::mousePressEvent(event);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    this->m_lock = {};
    this->unsetCursor();
    QMainWindow::mouseMoveEvent(event);
}

void MainWindow::customMouseMoveEvent(QMouseEvent *event){
    if (this->m_lock){
        QPoint tL = this->geometry().topLeft(), bR = this->geometry().bottomRight();
        if (this->m_lock & Qt::TopEdge) tL.ry() = event->globalY() - this->m_posCursor.y();
        if (this->m_lock & Qt::BottomEdge) bR.ry() = event->globalY() - this->m_posCursor.y();
        if (this->m_lock & Qt::LeftEdge) tL.rx() = event->globalX() - this->m_posCursor.x();
        if (this->m_lock & Qt::RightEdge) bR.rx() = event->globalX() - this->m_posCursor.x();
        this->setGeometry(QRect(tL, bR));
        return;
    }

    int x = event->globalX() - this->x(), y = event->globalY() - this->y();
    int bottom = this->height() - RESIZE_LIMIT, right = this->width() - RESIZE_LIMIT;

    if (x < RESIZE_LIMIT){
        if (y < RESIZE_LIMIT) this->setCursor(QCursor(Qt::SizeFDiagCursor));
        else if (y > bottom) this->setCursor(QCursor(Qt::SizeBDiagCursor));
        else this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (x > right){
        if (y < RESIZE_LIMIT) this->setCursor(QCursor(Qt::SizeBDiagCursor));
        else if (y > bottom) this->setCursor(QCursor(Qt::SizeFDiagCursor));
        else this->setCursor(QCursor(Qt::SizeHorCursor));
    }
    else if (y < RESIZE_LIMIT || y > bottom) this->setCursor(Qt::SizeVerCursor);
    else this->unsetCursor();
}

void MainWindow::mousePressEvent_2(QMouseEvent *e)
{
    isPressWidget = true;
    last = e->globalPos();
}

void MainWindow::mouseMoveEvent_2(QMouseEvent *e)
{
    if (isPressWidget)
    {
         int dx = e->globalX() - last.x();
         int dy = e->globalY() - last.y();
         last = e->globalPos();
         move(x()+dx, y()+dy);
    }
}

void MainWindow::mouseReleaseEvent_2(QMouseEvent *e)
{
    int dx = e->globalX() - last.x();
    int dy = e->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressWidget=false;
}

void MainWindow::on_pushButton_CodeRun_clicked()
{
    Interpreter::interpret(editor->text(), info.editor());
    editor->clearSearchIndic(0, editor->text().size());
    ui->plainTextEdit_terminal->appendPlainText(info.editor().toJson());
    ui->plainTextEdit_terminal->appendPlainText(info.editor().varsToJson());
    setErrorInfo(info.editor());
}

void MainWindow::updateEditorInfo()
{
    info.editor().updateText(editor->text());
}

void MainWindow::on_actionSave_As_A_triggered()
{
    if (Open_Location.isEmpty())
    {
        on_actionNew_N_triggered();
        return;
    }
    QString new_dir = QFileDialog::getSaveFileName(this,
                                                   tr("Save As"),
                                                   Open_Location,
                                                   tr("FRD File (*.frd);; FRD Json File (*.frdjson);; Json File (*.json)"));
    if (new_dir.isEmpty()) return;
    if (QFileInfo(new_dir).suffix() == "frd")
    {
        save_or_not = true;
        Open_Location = new_dir;
        setWindowTitle(QFileInfo(Open_Location).fileName() + " - <strong>Fractal Designer</strong>");
        on_actionSave_S_triggered();
    }
    info.print(new_dir);
}

void MainWindow::setErrorInfo(const FRD_Json& frd_json)
{
    QVector<FRD_Error> err_list = frd_json.errors();
    qDebug() << "Error " << frd_json.errors().size();
    error_list_model->setRowCount(err_list.size());
    for (int i = 0; i != err_list.size(); i++)
    {
        error_list_model->setItem(i, 0, new QStandardItem(err_list[i].error_type < 8000 ? "Error" : "Warning"));
        error_list_model->setItem(i, 1, new QStandardItem(QString::number(err_list[i].error_type)));
        error_list_model->setItem(i, 2, new QStandardItem(QString::number(err_list[i].row)));
        error_list_model->setItem(i, 3, new QStandardItem(QString::number(err_list[i].col)));
        error_list_model->setItem(i, 4, new QStandardItem(QString::number(err_list[i].length)));
        error_list_model->setItem(i, 5, new QStandardItem(err_list[i].msg));
    }
}

void MainWindow::on_pushButton_search_clicked()
{
    QString text = info.editor().text();
    editor->clearSearchIndic(0, text.size());
    QString str = ui->lineEdit_searchName->text();
    if (str.isEmpty()) return;
    int index = 0;
    auto cs = ui->checkBox_searchCaseSensitive->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive;
    while((index = text.indexOf(str, index, cs)) != -1)
    {
        editor->setSearchIndic(index, str.length());
        index += str.length();
    }
}

void MainWindow::on_lineEdit_searchName_returnPressed()
{
    on_pushButton_search_clicked();
}
