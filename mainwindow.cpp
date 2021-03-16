#include <QLabel>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QFile>
#include <QThread>
#include <QSettings>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << QThread::currentThreadId();
    QLabel *permanent = new QLabel(this);
    permanent->setText(tr("ALL RIGHTS RESERVED (C) 2021 Teddy van Jerry"));
    ui->statusbar->addPermanentWidget(permanent);
    ui->statusbar->showMessage(tr("Welcome to the Fractal Designer!"), 20000);
    show_template_graph();
    show_preview_image();
    Project_Name = "Unsaved project";
    Project_Template = "Undefined";
    setWindowTitle("Fractal Designer - " + Project_Name);

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
    Button_Login_MainWindow->setText("Log In");
    QFile Button_Login_MainWindow_qss(":/StyleSheet/Button_Login_MainWindow.qss");
    Button_Login_MainWindow_qss.open(QFile::ReadOnly);
    QString Button_Login_MainWindow_qss_str = QLatin1String(Button_Login_MainWindow_qss.readAll());
    Button_Login_MainWindow_qss.close();
    Button_Login_MainWindow->setStyleSheet(Button_Login_MainWindow_qss_str);
    ui->toolBar->insertWidget(NULL, Button_Login_MainWindow);
    connect(Button_Login_MainWindow, SIGNAL(clicked()), this, SLOT(on_Button_Login_MainWindow_clicked()));

    Label_User_Name = new QLabel(this);
    Label_User_Name->setSizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
    Label_User_Name->setStyleSheet("color: green; font: Consolas; font-size: 14px;");
    ui->toolBar->insertWidget(NULL, Label_User_Name);

    //setStyleSheet("background-color: ");
    //qDebug() << QCoreApplication::applicationDirPath();
    QString filename1(":/Templates/Template_1.bmp");
    if(!(image_T1.load(filename1))) // load the image
    {
        QMessageBox::information(this,
                     tr("Build Error"),
                     tr("Fail to open the image"));
    }
    QString filename2(":/Templates/Template_2.jpg");
    if(!(image_T2.load(filename2))) // load the image
    {
        QMessageBox::information(this,
                     tr("Build Error"),
                     tr("Fail to open the image"));
    }
    //ui->progressBar_Preview->setVisible(false);

    model->setColumnCount(6);
    model->setHeaderData(0, Qt::Horizontal, "t");
    model->setHeaderData(1, Qt::Horizontal, "x");
    model->setHeaderData(2, Qt::Horizontal, "y");
    model->setHeaderData(3, Qt::Horizontal, "angle");
    model->setHeaderData(4, Qt::Horizontal, "width");
    model->setHeaderData(5, Qt::Horizontal, "v rate");
    QStyledItemDelegate* preciseDoubleDelegate = new QStyledItemDelegate(ui->tableView_Route);
    preciseDoubleDelegate->setItemEditorFactory(&m_factory);
    ui->tableView_Route->setItemDelegate(preciseDoubleDelegate);
    ui->tableView_Route->setModel(model);
    ui->tableView_Route->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_Route->setSortingEnabled(true);
    model->setSortRole(Qt::AscendingOrder);
    //ui->tableView_Route->setItemDelegate;
    connect(model, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(routeEdit(QStandardItem*)));
    connect(ui->tableView_Route->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(dealRouteSort(int)));

    ui->actionStop->setDisabled(true);

    route_tool_window = new Route_Tool(this);

    show_preview_image();
    showColourFormula();
}

MainWindow::~MainWindow()
{
    //delete image;
    delete ui;
}

void MainWindow::setOpenLocation(QString str)
{
    Open_Location = str;
    if(str != "")
    {
        Version_Higher_Than_4 = true;
        save_or_not = true;
        High_Version_Open(OPEN_FILE_OUT);
    }
    else
    {

    }
}

bool MainWindow::High_Version_Open(int type)
{
    ui->actionClose->setEnabled(true);

    QFile FRD_R(Open_Location);
    FRD_R.open(QIODevice::ReadOnly);
    FRD_R.skip(16);
    if(FRD_R.read(8) != "FRD\xEFTvJ*")
    {
        QMessageBox::critical(this, "Fail to open", "The file may be damaged or should not be opened with Fractal Designer.");
        if(type) // OPEN_FILE_OUT
        {
            exit(1);
        }
        else
        {
            save_or_not = false;
            return false;
        }
    }
    save_or_not = true;
    char name_1 = 0, name_2 = 0;
    qint32 length_;

    current_info_v = 0;
    redo_max_depth = 1;
    qDebug() << FRD_R.read(4);
    QDataStream in(&FRD_R);
    while(!FRD_R.atEnd())
    {
        in >> name_1 >> name_2;
        if NameIs('T', 'E')
        {
            FRD_R.skip(4);
            in >> curr_info.template_;
            if(curr_info.template_ == 0) redo_max_depth++;
            qDebug() << "TE succeeds!";
        }
        else if NameIs('I', 'V')
        {
            FRD_R.skip(4);
            in >> curr_info.min_class_v >> curr_info.max_class_v >> curr_info.max_loop_t;
        }
        else if NameIs('C', '1')
        {
            FRD_R.skip(4);
            For_All_Colour(i, j)
                in >> curr_info.Colour_Data_1[i][j][0] >> curr_info.Colour_Data_1[i][j][1];
            End_All_Colour
        }
        else if NameIs('C', '2')
        {
            FRD_R.skip(4);
            For_All_Colour(i, j)
                in >> curr_info.Colour_Data_2[i][j][0] >> curr_info.Colour_Data_2[i][j][1];
            End_All_Colour
        }
        else if NameIs('R', 'O')
        {
            in >> length_;
            // qDebug() << length_;
            for(int i = 0; i != length_ / _ROUTE_SIZE_; i++)
            {
                Route_Info temp_info;
                curr_info.Route_.push_back(temp_info.read(in));
            }
            // qDebug() << curr_info.Route_.size();
        }
        else if NameIs('I', 'O')
        {
            FRD_R.skip(4);
            in >> curr_info.image_size_x >> curr_info.image_size_y
               >> curr_info.frame_rate_index >> curr_info.total_time_str
               >> curr_info.img_path >> curr_info.img_prefix;
        }
        else if NameIs('V', 'I')
        {
            FRD_R.skip(4);
            quint16 list_size = 0;
            in >> curr_info.video_path >> curr_info.video_name >> list_size;
            curr_info.music_list.resize(list_size);
            for(int i = 0 ; i != list_size; i++)
            {
                in >> curr_info.music_list[i];
            }
        }
        else if NameIs('C', 'O')
        {
            qDebug() << "Config" << (int)curr_info.config1;
            FRD_R.skip(4);
            in >> curr_info.config1;
            if((curr_info.config1 & 1) == 1)
            {
                ui->actionAuto_Refresh->setChecked(true);
            }
        }
        else
        {
            in >> length_;
            FRD_R.skip(length_);
        }
    }

    FRD_R.close();
    qDebug() << "High Version Open succeed!";

    display();
    save_or_not = true;

    setWindowTitle("Fractal Designer - " + Open_Location);
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
    QDesktopServices::openUrl(QUrl("https://blog.csdn.net/weixin_50012998/article/details/114821562"));
}

void MainWindow::on_MainWindow_AboutTVJ_clicked()
{
    QDesktopServices::openUrl(QUrl("https://blog.csdn.net/weixin_50012998"));
}

void MainWindow::on_pushButton_clicked()
{
    MainWindow::on_actionChinese_triggered();
}

void MainWindow::on_actionNew_N_triggered()
{
    if(Version_Higher_Than_4 && ui->actionClose->isEnabled())
    {
        on_actionClose_triggered();
    }
    Version_Higher_Than_4 = true;
    New_File* new_project = new New_File(this);
    new_project->show();
    save_or_not = true;
    if(Open_Location == "" && Project_Name != "Unsaved project")
    {
        Version_Higher_Than_4 = false;
    }
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
                                                 tr("FRD Files(*frd);"));
    qDebug() << New_Open_Location;
    QFile check(New_Open_Location);
    if(check.exists())
    {
        qDebug() << "exist";
        if(ui->actionClose->isEnabled())
        {
            on_actionClose_triggered();
        }
        Version_Higher_Than_4 = true;
        Open_Location = New_Open_Location;
        if(!High_Version_Open(OPEN_FILE_IN))
        {
            Open_Location = Old_Open_Location;
            // setWindowTitle("Fractal Designer - " + Open_Location);
        }
    }
    else if(New_Open_Location != "")
    {
        QMessageBox::warning(this, "Opening Project Error", "The file doesn't exist!");
        // Open_Location = "";
    }
    else if(Project_Name != "Unsaved project")
    {
        Version_Higher_Than_4 = false;
    }

}

void MainWindow::on_MainWindow_openfile_clicked()
{
    MainWindow::on_actionOpen_O_triggered();
}

void MainWindow::set_Colour_Dlg(int n)
{
    Set_Colour* set_colour_window = new Set_Colour(this, Project_Name, n);
//    set_colour_window->set_label(n);
    set_colour_window->show();
}

void MainWindow::on_Convergent_setColour_clicked()
{
    set_Colour_Dlg(0);
}

void MainWindow::on_Unconvergent_setColour_clicked()
{
    set_Colour_Dlg(1);
}

void MainWindow::show_template_graph()
{
//    ui->label_Template_1->setPixmap(QPixmap::fromImage());
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
    //Route_Tool* route_tool = new Route_Tool;
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

    ui->label_Template_1->resize(ui->widget_T1->size());
    int T1_w_width(ui->label_Template_1->width());
    int T1_w_height(ui->label_Template_1->height());
    double rate_w_T1 = static_cast<double>(T1_w_height) /T1_w_width;
    double rate_i_T1 = static_cast<double>(image_T1.height()) / image_T1.width();
    if(rate_i_T1 > rate_w_T1)
    {

        ui->label_Template_1->setPixmap(QPixmap::fromImage(image_T1).scaledToHeight(T1_w_height));
    }
    else
    {
        ui->label_Template_1->setPixmap(QPixmap::fromImage(image_T1).scaledToWidth(T1_w_width));
    }

    ui->label_Template_2->resize(ui->widget_T2->size());
    int T2_w_width(ui->label_Template_2->width());
    int T2_w_height(ui->label_Template_2->height());
    double rate_w_T2 = static_cast<double>(T2_w_height) /T2_w_width;
    double rate_i_T2 = static_cast<double>(image_T2.height()) / image_T2.width();
    if(rate_i_T2 > rate_w_T2)
    {

        ui->label_Template_2->setPixmap(QPixmap::fromImage(image_T2).scaledToHeight(T1_w_height));
    }
    else
    {
        ui->label_Template_2->setPixmap(QPixmap::fromImage(image_T2).scaledToWidth(T1_w_width));
    }
}

void MainWindow::on_Tab_currentChanged(int index)
{
    if(index == 2)
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
    if(index == 1)
    {
        ui->label_Template_1->resize(ui->widget_T1->size());
        int T1_w_width(ui->label_Template_1->width());
        int T1_w_height(ui->label_Template_1->height());
        double rate_w_T1 = static_cast<double>(T1_w_height) / T1_w_width;
        double rate_i_T1 = static_cast<double>(image_T1.height()) / image_T1.width();
        if(rate_i_T1 > rate_w_T1)
        {
            ui->label_Template_1->setPixmap(QPixmap::fromImage(image_T1).scaledToHeight(T1_w_height));
        }
        else
        {
            ui->label_Template_1->setPixmap(QPixmap::fromImage(image_T1).scaledToWidth(T1_w_width));
        }

        ui->label_Template_2->resize(ui->widget_T2->size());
        int T2_w_width(ui->label_Template_2->width());
        int T2_w_height(ui->label_Template_2->height());
        double rate_w_T2 = static_cast<double>(T2_w_height) /T2_w_width;
        double rate_i_T2 = static_cast<double>(image_T2.height()) / image_T2.width();
        if(rate_i_T2 > rate_w_T2)
        {

            ui->label_Template_2->setPixmap(QPixmap::fromImage(image_T2).scaledToHeight(T1_w_height));
        }
        else
        {
            ui->label_Template_2->setPixmap(QPixmap::fromImage(image_T2).scaledToWidth(T1_w_width));
        }
    }
}

void MainWindow::on_actionSave_S_triggered()
{
    HIGH_V_ONLY
    save_or_not = true;
    if(Open_Location == "") on_actionNew_N_triggered();

    // Preparation configuration
    curr_info.config1 = 0;
    if(ui->actionAuto_Refresh->isChecked())
    {
        curr_info.config1 = 1;
    }

    // == Print into file ==
    curr_info.print(Open_Location, FRD_Version);
}

void MainWindow::on_Template_Choice_1_toggled(bool checked)
{
    if(checked == true)
    {
        Project_Template = "1";
        //edit();
    }
}

void MainWindow::on_Template_Choice_2_toggled(bool checked)
{
    if(checked == true)
    {
        Project_Template = "2";
        //edit();
    }
}

void MainWindow::on_Template_Choice_3_toggled(bool checked)
{
    if(checked == true)
    {
        Project_Template = "3";
        //edit();
    } 
}

void MainWindow::on_Template_Choice_4_toggled(bool checked)
{
    if(checked == true)
    {
        Project_Template = "4";
        //edit();
    }    
}

void MainWindow::on_actionPreview_Refresh_triggered()
{
    Create_Image_Task* preview = new Create_Image_Task(this);

    Create_Images_Task_Pre(preview);

    QString Pre_Img_Dir;
    if(Version_Higher_Than_4)
    {
        QDir ck(QCoreApplication::applicationDirPath() + "/temp");
        if(!ck.exists())
        {
            ck.mkdir(ck.absolutePath());
        }
        Pre_Img_Dir = QCoreApplication::applicationDirPath() + "/temp";
    }
    else
    {
        Pre_Img_Dir = Project_Name;
    }
    preview->setImage(-0.7, 0, 3.2, 2.4, 800, 600, 0, ui->doubleSpinBox_t->value(), "png", Pre_Img_Dir, "Preview Image", "Preview");
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
            if(Version_Higher_Than_4)
            {
                on_actionSave_S_triggered();
            }
            else
            {
                on_actionSave_triggered();
                /*
                New_File* new_project = new New_File(this);
                new_project->show();
                new_project->exec();
                save_or_not = true;
                */
            }
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
        model->setItem(i / 6, i % 6, new QStandardItem(QString::number(vec[i], 'g', 11)));
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
    for(auto line : vec)
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
    if(ui->actionAuto_Refresh->isChecked())
    {
        MainWindow::on_actionPreview_Refresh_triggered();
    }
}

void MainWindow::on_pushButton_routeAdd_clicked()
{
    int row_count_now = model->rowCount();
    for(int i = 0; i != 6; i++)
    {
        model->setItem(row_count_now, i, new QStandardItem("0"));
        //qDebug() << "alive";
    }
}

void MainWindow::on_pushButton_routeRemove_clicked()
{
    model->removeRow(model->rowCount() - 1);
    edit();
    display();
}

void MainWindow::on_pushButton_routeClear_clicked()
{
    model->setRowCount(0);
    edit();
}

void MainWindow::on_actionCreate_Images_triggered()
{
    if(!ui->actionCreate_Images->isEnabled()) return;
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

    if((!Version_Higher_Than_4 && (Project_Template == "" || Project_Template == "Undefined")) || (Version_Higher_Than_4 && Open_Location == ""))
    {
        QMessageBox::warning(this, "Can not create images", "You have not chosen a template.");
        return;
    }

    if(Version_Higher_Than_4 && !isRouteValid)
    {
        QMessageBox::warning(this, "Can not create images", "The Route Settings are invalid.");
        return;
    }

    QString image_format = "png";
    QString path = ui->lineEdit_imagePath->text();
    QString name = ui->lineEdit_imagePrefix->text();
    if(!QDir(path).exists())
    {
        QMessageBox::warning(this, "Can not create images", "The path does not exist.");
        return;
    }

    ui->actionStop->setDisabled(false);
    ui->actionCreate_Images->setDisabled(true);
    ui->actionCreate_Images_in_Range->setDisabled(true);
    ui->actionCheck_Images->setDisabled(true);
    // on_actionPreview_Refresh_triggered();
    // work_thread->start();
    qDebug() << ui->timeEdit->time().second() + 60 * ui->timeEdit->time().minute();
    int total_image = ui->comboBox_fps->currentText().toInt() * (ui->timeEdit->time().second() + 60 * ui->timeEdit->time().minute());
    int current_index = 0;
    int X = ui->Image_size_X->value();
    int Y = ui->Image_size_Y->value();

    create_image_info = new Create_Image_Info;
    connect(this, &MainWindow::build_image_info_signal, create_image_info, &Create_Image_Info::set_info);
    connect(this, &MainWindow::build_image_updateInfo_signal, create_image_info, &Create_Image_Info::updateInfo);
    //connect(create_image_info, &Create_Image_Info::close, create_image_info, &Create_Image_Info::close_create_image_info);
    create_image_info->show();
    emit build_image_info_signal(path + "\\" + name, image_format, total_image, 0);

    /* Old Version
    for(int i = 0; i < total_image - 1; i++)
    {        
        double t = static_cast<double>(i) / total_image;
        while(current_index + 2 < model->rowCount() && t > model->item(current_index + 1)->text().toDouble())
        {
            current_index++;
        }
        double dt = (t - model->item(current_index, 0)->text().toDouble())
                / (model->item(current_index + 1, 0)->text().toDouble() - model->item(current_index, 0)->text().toDouble());
        double k = (model->item(current_index, 5)->text().toDouble());
        double x = model->item(current_index, 1)->text().toDouble()
                + (model->item(current_index + 1, 1)->text().toDouble() - model->item(current_index, 1)->text().toDouble())
                * ((1 - k) * dt + k * pow(dt, 2));
        double y = model->item(current_index, 2)->text().toDouble()
                + (model->item(current_index + 1, 2)->text().toDouble() - model->item(current_index, 2)->text().toDouble())
                * ((1 - k) * dt + k * pow(dt, 2));
        double angle = model->item(current_index, 3)->text().toDouble()
                + (model->item(current_index + 1, 3)->text().toDouble() - model->item(current_index, 3)->text().toDouble())
                * ((1 - k) * dt + k * pow(dt, 2));
        double width = 1.0 / (1.0 / model->item(current_index, 4)->text().toDouble()
                + (1.0 / model->item(current_index + 1, 4)->text().toDouble() - 1.0 / model->item(current_index, 4)->text().toDouble())
                * ((1 - k) * dt + k * pow(dt, 2)));
        qDebug() << t << current_index << x << y << width << angle;
        emit build_signal(x, y, width, width * Y / X, X, Y, angle, t, image_format, Project_Name, name + QString::number(i), "Create_Image");
    }
    */

    // New version with the normal speed

    for(int i = 0; i < total_image - 1; i++)
    {
        double T = static_cast<double>(i) / total_image;
        while(current_index + 2 < model->rowCount() && T > Tb(current_index + 1, 0))
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
        // create_images->setAutoDelete(false);
        Create_Images_Task_Pre(create_images);
        create_images->setImage(x, y, width, width * Y / X, X, Y, angle, T, image_format, path, name + QString::number(i), "Create_Image");
        QThreadPool::globalInstance()->start(create_images);
    }

    Create_Image_Task* create_images = new Create_Image_Task(this);
    // create_images->setAutoDelete(false);
    Create_Images_Task_Pre(create_images);
    create_images->setImage(Tb(model->rowCount() - 1, 1),
                            Tb(model->rowCount() - 1, 2),
                            Tb(model->rowCount() - 1, 4),
                            Tb(model->rowCount() - 1, 4) * Y / X,
                            X, Y,
                            Tb(model->rowCount() - 1, 3),
                            1, image_format, path, name + QString::number(total_image - 1), "Create_Image_Last");
    QThreadPool::globalInstance()->start(create_images);
    qDebug() << "Here Here !!!!";

    // delete create_images;
}

void MainWindow::on_commandLinkButton_Image_clicked()
{
    MainWindow::on_actionCreate_Images_triggered();
}

void MainWindow::on_Slider_t_sliderReleased()
{
    if(ui->actionAuto_Refresh->isChecked())
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
    QString default_dir = Project_Name;
    if(Version_Higher_Than_4)
    {
        default_dir = Open_Location;
        while(default_dir.right(1) != "/" && default_dir.right(1) != "\\")
        {
            default_dir.chop(1);
        }
        default_dir.chop(1);
    }
    QString Pro_Path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Open Project"), QDir::fromNativeSeparators(default_dir)));
    ui->toolButton_imagePath->setDisabled(false);
    ui->lineEdit_imagePath->setText(Pro_Path);
}

void MainWindow::on_actionCreate_Video_triggered()
{
    if(User_Name.isEmpty())
    {
        QMessageBox::information(this, "Information", "You have not logged in.");
        return;
    }

    if(!save_or_not)
    {
        QMessageBox::information(this, "Information", "You have not saved the project.");
        return;
    }

//    QMessageBox::information(this, "Information", "Creating video...", QMessageBox::NoButton);

    int crf_value = 18;
    QString video_file_name = ui->lineEdit_videoName->text();
    QString video_file_path = ui->lineEdit_videoPath->text(); // only used in high version
    QString video_format = "mp4";
    QString PowerShell_arg1 = "ffmpeg -r ";
    PowerShell_arg1.append(ui->comboBox_fps->currentText());
    PowerShell_arg1.append(" -f image2 -i ");
    PowerShell_arg1.append(tr("\"") + ui->lineEdit_imagePath->text() + "/" + ui->lineEdit_imagePrefix->text());
    PowerShell_arg1.append("%d.png\" -vcodec libx264 -crf ");
    PowerShell_arg1.append(QString::number(crf_value));
    PowerShell_arg1.append(" -pix_fmt yuv420p ");
    PowerShell_arg1.append(ui->textBrowser_music->toPlainText().isEmpty() ?
                               video_file_name + "." + video_format
                             : video_file_name + "_temp." + video_format);

    if(Version_Higher_Than_4)
    {
        bool Alive[3] {false, false, false};
        QFile video(video_file_path + "/" + video_file_name + "." + video_format);
        if(video.exists())
        {
            if(QMessageBox::question(this, "Choice", tr("The video ") + video_file_name + "." + video_format
                                     + " already exists.\nDo you want to overwrite it?") == QMessageBox::Yes)
            {
                video.remove();
            }
            else return;
        }
        QProcess* create_video = new QProcess;
        create_video->setWorkingDirectory(video_file_path);
        QString cmd("powershell");
        QStringList parameters1{PowerShell_arg1};
        create_video->start(cmd, parameters1);
        Alive[0] = create_video->waitForFinished(60000);

        if(!Alive[0])
        {
            QFile create_video_ps(video_file_path + "/Create_Video.ps1");
            create_video_ps.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream out1(&create_video_ps);
            out1 << PowerShell_arg1;
            create_video_ps.close();
        }

        if(ui->textBrowser_music->toPlainText().isEmpty())
        {
            if(Alive[0])
            {
                QMessageBox::information(this, "Information", "Video Creating Finished!");
            }
            else
            {
                QMessageBox::warning(this, "Error Information", "Video Creation failed!\nYou can run Create_Video.ps1 code instead.");
            }
            return;
        }

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

        QString PowerShell_arg2 = "ffmpeg -f concat -i Music_Added_NoEnd.txt -c copy BGM.mp3";
        QStringList parameters2{PowerShell_arg2};
        create_video->start(cmd, parameters2);
        Alive[1] = create_video->waitForFinished(60000);

        QString PowerShell_arg3 = "ffmpeg -i BGM.mp3 -i ";
        PowerShell_arg3.append(video_file_name + "_temp." + video_format);
        PowerShell_arg3.append(" -shortest -f mp4 ");
        PowerShell_arg3.append(video_file_name + "." + video_format);
        if(Alive[0] && Alive[1])
        {
            QStringList parameters3(PowerShell_arg3);
            create_video->start(cmd, parameters3);
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
            QFile add_music_ps(video_file_path + "/Add_Music.ps1");
            add_music_ps.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream out2(&add_music_ps);
            out2 << PowerShell_arg2 << Qt::endl;
            out2 << PowerShell_arg3;
            add_music_ps.close();
            if(Alive[0])
            {
                QMessageBox::warning(this, "Error Information", "Video Creation failed!\nYou can run Add_Music.ps1 code instead.");
            }
            else
            {
                QMessageBox::warning(this, "Error Information", "Video Creation failed!\nFirst run Create_Video.ps1,\nsecond run Add_Music.ps1.");
            }
        }
        else
        {
            QMessageBox::information(this, "Information", "Video Creation Finished!");
        }
    }
    else
    {
        QFile video(Project_Name + "/" + video_file_name + "." + video_format);
        if(video.exists())
        {
            if(QMessageBox::question(this, "Choice", tr("The video ") + video_file_name + "." + video_format
                                     + " already exists.\nDo you want to overwrite it?") == QMessageBox::Yes)
            {
                video.remove();
            }
            else return;
        }
        QProcess* create_video = new QProcess;
        create_video->setWorkingDirectory(Project_Name);
        QString cmd("powershell");
        QStringList parameters1{PowerShell_arg1};
        create_video->start(cmd, parameters1);
        create_video->waitForFinished();

        QFile create_video_ps(Project_Name + "/Create_Video.ps1");
        create_video_ps.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out1(&create_video_ps);
        out1 << PowerShell_arg1;
        create_video_ps.close();

        QFile Music_Added_NoEnd_(Project_Name + "/Music_Added_NoEnd.txt");
        Music_Added_NoEnd_.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&Music_Added_NoEnd_);
        QFile music_bro(Project_Name + "/Music_Added.txt");
        if(music_bro.exists())
        {
            int cnt = 1;
            QVector<QString> contents;
            QString str;
            QTextStream in(&music_bro);
            music_bro.open(QIODevice::ReadOnly | QIODevice::Text);
            while (cnt <= 100)
            {
                str = in.readLine();
                if(str == "<End>") break;
                QFile::remove(Project_Name + "/" + QString::number(cnt) + ".mp3"); // tomorrow think about replacing this mp3
                if(QFile::copy(str, Project_Name + "/" + QString::number(cnt) + ".mp3"))
                {
                    out << "file '" << QString::number(cnt) + ".mp3'" << Qt::endl;
                    qDebug() << "succeeded copy music_added";
                }
                else
                {
                    qDebug() << "failed copy music_added";
                }
                cnt++;
            }
            music_bro.close();
            Music_Added_NoEnd_.close();
        }

        QString PowerShell_arg2 = "ffmpeg -f concat -i Music_Added_NoEnd.txt -c copy BGM.mp3";
        QStringList parameters2{PowerShell_arg2};
        create_video->start(cmd, parameters2);
        create_video->waitForFinished();

        QString PowerShell_arg3 = "ffmpeg -i BGM.mp3 -i ";
        PowerShell_arg3.append(video_file_name + "_temp." + video_format);
        PowerShell_arg3.append(" -shortest -f mp4 ");
        PowerShell_arg3.append(video_file_name + "." + video_format);
        QStringList parameters3(PowerShell_arg3);
        create_video->start(cmd, parameters3);
        create_video->waitForFinished(-1);

        QFile::remove(Project_Name + "/" + video_file_name + "_temp." + video_format);
        QFile::remove(Project_Name + "/BGM.mp3");
        for(int i = 1; i <= 100; i++)
        {
            QFile::remove(Project_Name + "/" + QString::number(i) + ".mp3");
        }

        QFile add_music_ps(Project_Name + "/Add_Music.ps1");
        add_music_ps.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out2(&add_music_ps);
        out2 << PowerShell_arg2 << Qt::endl;
        out2 << PowerShell_arg3;
        add_music_ps.close();

        QMessageBox::information(this, "Information", "Video Creating Finished!");
    }

    qDebug() << "Create Video:" << PowerShell_arg1;

}

void MainWindow::on_toolButton_videoPath_clicked()
{
    QString default_dir = Version_Higher_Than_4 ? QDir::homePath() : Project_Name;
    QString video_Path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Choose Path"), QDir::fromNativeSeparators(default_dir)));
    ui->toolButton_imagePath->setDisabled(false);
    if(video_Path == "") return;
    ui->lineEdit_videoPath->setText(video_Path);
    edit();
}

void MainWindow::on_actionEnglish_triggered()
{
    Help* help = new Help;
    help->show();
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
                                                    tr("Music Files(*mp3);"));
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
    if(Version_Higher_Than_4)
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
    else
    {
        int total_index = 0;
        QFile in_unfirom(Project_Name + "/Route_Info.txt");
        if(in_unfirom.exists())
        {
            in_unfirom.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&in_unfirom);
            in >> total_index;
            in_unfirom.close();
            qDebug() << "Total_number:" << total_index;
        }
        double* L = new double[total_index - 1];
        double* L_till = new double[total_index - 1];
        double total_L = 0;
        for(int current_index = 0; current_index != total_index - 1; current_index++)
        {
            double w1 = model->item(current_index, 4)->text().toDouble();
            double w2 = model->item(current_index + 1, 4)->text().toDouble();

            double x1 = model->item(current_index, 1)->text().toDouble();
            double x2 = model->item(current_index + 1, 1)->text().toDouble();
            double y1 = model->item(current_index, 2)->text().toDouble();
            double y2 = model->item(current_index + 1, 2)->text().toDouble();
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
        for(int current_index = 0; current_index != total_index - 1; current_index++)
        {
            model->item(current_index, 0)->setText(QString::number(L_till[current_index] / total_L, 'g', 10));
            model->item(current_index, 5)->setText("0");
        }
        model->item(total_index - 1, 0)->setText("1");
        model->item(total_index - 1, 5)->setText("0");
        delete[] L;
        delete[] L_till;
    }
}

void MainWindow::on_actionFFmpeg_triggered()
{
    QProcess FFmpeg_Process(this);
    QString programme = "Resources/ffmpeg.exe";
    // run the programme FFmpeg
    FFmpeg_Process.start(programme);
    if(FFmpeg_Process.waitForFinished(5000))
    {
        QMessageBox::information(this, "Information", "Successfully install FFmpeg!");
    }
    else
    {
        QMessageBox::warning(this, "Information", "Failed to install FFmpeg!");
    }
}

void MainWindow::on_actionCheck_Update_triggered()
{
    QDesktopServices::openUrl(QUrl("https://blog.csdn.net/weixin_50012998/article/details/113389239"));
}

void MainWindow::on_actionAuto_Refresh_triggered()
{
    /*
    if(ui->actionAuto_Refresh->isChecked())
    {
        ui->actionAuto_Refresh->setChecked(false);
        // curr_info.config1 = 0;
    }
    else
    {
        ui->actionAuto_Refresh->setChecked(true);
        // curr_info.config1 = 1;
    }
    */
}

void MainWindow::on_actionNew_triggered()
{
    Version_Higher_Than_4 = false;
    New_File* new_project = new New_File(this);
    new_project->show();
    save_or_not = true;
    if(Open_Location != "" && Project_Name == "Unsaved project")
    {
        Version_Higher_Than_4 = true;
    }
}

void MainWindow::on_actionOpen_triggered()
{
    Version_Higher_Than_4 = false;
    Open_File* open_project = new Open_File(this);
    connect(open_project, &Open_File::template_choice, this, &MainWindow::iniTemplate);
    connect(open_project, &Open_File::define_value, this, &MainWindow::iniValue);
    connect(open_project, &Open_File::image_size, this, &MainWindow::iniSize);
    connect(open_project, &Open_File::route_info, this, &MainWindow::iniRouteInfo);
    connect(open_project, &Open_File::image_path, this, &MainWindow::iniImagePath);
    connect(open_project, &Open_File::image_prefix, this, &MainWindow::iniImagePrefix);
    connect(open_project, &Open_File::frame_rate, this, &MainWindow::iniFrameRate);
    connect(open_project, &Open_File::total_time, this, &MainWindow::iniTotalTime);
    connect(open_project, &Open_File::music_added, this, &MainWindow::iniMusicAdded);
    open_project->show();
    open_project->exec();
    save_or_not = true;
    MainWindow::on_actionSave_triggered();
    if(Open_Location != "" && Project_Name == "")
    {
        Version_Higher_Than_4 = true;
    }
}

void MainWindow::on_actionSave_triggered()
{
    if(!save_or_not)
    {
        New_File* new_file = new New_File(0);
        new_file->setWindowIcon(QIcon(":/exe_icon/EXE Icons/FRD_icon.png"));
        new_file->open();
    }

    LOW_V_ONLY

    QFile Template_Choice(Project_Name + "/Template_Choice.txt");
    Template_Choice.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out1(&Template_Choice);
    out1 << Project_Template;
    Template_Choice.close();

    QFile Define_Value(Project_Name + "/Define_Value.txt");
    Define_Value.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out2(&Define_Value);
    out2 << ui->Min_class_value->value() << tr(" ") << ui->Max_class_value->value() << tr(" ") << ui->Max_loop_time->value();
    Define_Value.close();

    QFile Image_Size(Project_Name + "/Image_Size.txt");
    Image_Size.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out3(&Image_Size);
    out3 << ui->Image_size_X->value() << tr(" ") << ui->Image_size_Y->value();
    Image_Size.close();

    QFile Route_Info_Param(Project_Name + "/Route_Info.txt");
    Route_Info_Param.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out4(&Route_Info_Param);
    out4 << model->rowCount() << Qt::endl;
    for(int i = 0; i != model->rowCount(); i++)
    {
        qDebug() << "save the table";
        for(int j = 0; j != 6; j++)
        {
            out4 << model->item(i, j)->text() << " ";
        }
        out4 << Qt::endl;
    }
    Route_Info_Param.close();

    QFile Image_Prefix_(Project_Name + "/Image_Prefix.txt");
    Image_Prefix_.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out5(&Image_Prefix_);
    out5 << ui->lineEdit_imagePrefix->text();
    Image_Prefix_.close();

    QFile Image_Dir_(Project_Name + "/Image_Dir.txt");
    Image_Dir_.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out6(&Image_Dir_);
    out6 << ui->lineEdit_imagePath->text();
    Image_Dir_.close();

    QFile Frame_Rate_(Project_Name + "/Frame_Rate.txt");
    Frame_Rate_.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out7(&Frame_Rate_);
    out7 << ui->comboBox_fps->currentIndex();
    Frame_Rate_.close();

    QFile Total_Time_(Project_Name + "/Total_Time.txt");
    Total_Time_.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out8(&Total_Time_);
    out8 << ui->timeEdit->text();
    Total_Time_.close();

    QFile Music_Added_(Project_Name + "/Music_Added.txt");
    Music_Added_.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out9(&Music_Added_);
    out9 << ui->textBrowser_music->toPlainText() << "\n<End>";
    Music_Added_.close();

    QFile Colour_Set_1(Project_Name + "/Colour_Set_1.txt");
    QFile Colour_Set_temp1(Project_Name + "/Colour_Set_temp1.txt");
    if(Colour_Set_temp1.exists())
    {
        QFile::remove(Project_Name + "/Colour_Set_1.txt");
        if(QFile::copy(Project_Name + "/Colour_Set_temp1.txt", Project_Name + "/Colour_Set_1.txt"))
        {
            qDebug() << "succeeded copy";
        }
        else
        {
            qDebug() << "failed copy";
        }
    }

    QFile Colour_Set_2(Project_Name + "/Colour_Set_2.txt");
    QFile Colour_Set_temp2(Project_Name + "/Colour_Set_temp2.txt");
    if(Colour_Set_temp2.exists())
    {
        QFile::remove(Project_Name + "/Colour_Set_2.txt");
        if(QFile::copy(Project_Name + "/Colour_Set_temp2.txt", Project_Name + "/Colour_Set_2.txt"))
        {
            qDebug() << "succeeded copy";
        }
        else
        {
            qDebug() << "failed copy";
        }
    }

    QString Formula[2][29] = {
        {
            "k",
            "Zn.X",
            "Zn.Y",
            "|Zn|",
            "|Zn| / MIN",
            "argz(Zn)",
            "sin(argz(Zn))",
            "cos(argz(Zn))",
            "Z0.X",
            "Z0.Y",
            "|Z0|",
            "|Z0| / MIN",
            "argz(Z0)",
            "sin(argz(Z0))",
            "|Zn-Z0|",
            "e^k",
            "10^k",
            "ln(1+|Zn|)",
            "ln(1+|Z0|)",
            "ln(1+|Zn-Z0|)",
            "e^|Zn|",
            "e^|Z0|",
            "e^|Zn-Z0|",
            "10^|Zn|",
            "10^|Z0|",
            "10^|Zn-Z0|",
            "e^(|Zn| / MIN)",
            "e^(|Z0| / MIN)",
            ""
        },
        {
            "k",
            "Zn.X",
            "Zn.Y",
            "|Zn|",
            "|Zn| / MAX",
            "argz(Zn)",
            "sin(argz(Zn))",
            "cos(argz(Zn))",
            "Z0.X",
            "Z0.Y",
            "|Z0|",
            "|Z0| / MAX",
            "argz(Z0)",
            "sin(argz(Z0))",
            "|Zn-Z0|",
            "e^k",
            "10^k",
            "ln(1+|Zn|)",
            "ln(1+|Z0|)",
            "ln(1+|Zn-Z0|)",
            "e^|Zn|",
            "e^|Z0|",
            "e^|Zn-Z0|",
            "10^|Zn|",
            "10^|Z0|",
            "10^|Zn-Z0|",
            "e^(|Zn| / MAX)",
            "e^(|Z0| / MAX)",
            ""
        }
    };

    QString colour_defined[2][4] = {
        {"R = ", "G = ", "B = ", "A = "},
        {"R = ", "G = ", "B = ", "A = "}
    };
    double Colour_Data[2][4][29][2] = {0};
    //Colour_Data[0][3][28][1] = Colour_Data[1][3][28][1] = 255;

    QFile Colour_saved_1(Project_Name + "/Colour_Set_temp1.txt");
    if(Colour_saved_1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in1(&Colour_saved_1);
        for(int j = 0; j != 4; j++)
        {
            in1 >> Colour_Data[0][j][0][0] >> Colour_Data[0][j][0][1];
            in1 >> Colour_Data[0][j][1][0] >> Colour_Data[0][j][1][1];
            in1 >> Colour_Data[0][j][2][0] >> Colour_Data[0][j][2][1];
            in1 >> Colour_Data[0][j][3][0] >> Colour_Data[0][j][3][1];
            in1 >> Colour_Data[0][j][4][0] >> Colour_Data[0][j][4][1];
            in1 >> Colour_Data[0][j][5][0] >> Colour_Data[0][j][5][1];
            in1 >> Colour_Data[0][j][6][0] >> Colour_Data[0][j][6][1];
            in1 >> Colour_Data[0][j][7][0] >> Colour_Data[0][j][7][1];
            in1 >> Colour_Data[0][j][8][0] >> Colour_Data[0][j][8][1];
            in1 >> Colour_Data[0][j][9][0] >> Colour_Data[0][j][9][1];
            in1 >> Colour_Data[0][j][10][0] >> Colour_Data[0][j][10][1];
            in1 >> Colour_Data[0][j][11][0] >> Colour_Data[0][j][11][1];
            in1 >> Colour_Data[0][j][12][0] >> Colour_Data[0][j][12][1];
            in1 >> Colour_Data[0][j][13][0] >> Colour_Data[0][j][13][1];
            in1 >> Colour_Data[0][j][14][0] >> Colour_Data[0][j][14][1];
            in1 >> Colour_Data[0][j][15][0] >> Colour_Data[0][j][15][1];
            in1 >> Colour_Data[0][j][16][0] >> Colour_Data[0][j][16][1];
            in1 >> Colour_Data[0][j][17][0] >> Colour_Data[0][j][17][1];
            in1 >> Colour_Data[0][j][18][0] >> Colour_Data[0][j][18][1];
            in1 >> Colour_Data[0][j][19][0] >> Colour_Data[0][j][19][1];
            in1 >> Colour_Data[0][j][20][0] >> Colour_Data[0][j][20][1];
            in1 >> Colour_Data[0][j][21][0] >> Colour_Data[0][j][21][1];
            in1 >> Colour_Data[0][j][22][0] >> Colour_Data[0][j][22][1];
            in1 >> Colour_Data[0][j][23][0] >> Colour_Data[0][j][23][1];
            in1 >> Colour_Data[0][j][24][0] >> Colour_Data[0][j][24][1];
            in1 >> Colour_Data[0][j][25][0] >> Colour_Data[0][j][25][1];
            in1 >> Colour_Data[0][j][26][0] >> Colour_Data[0][j][26][1];
            in1 >> Colour_Data[0][j][27][0] >> Colour_Data[0][j][27][1];
            in1 >> Colour_Data[0][j][28][0] >> Colour_Data[0][j][28][1];
        }
        Colour_saved_1.close();
    }
    else
    {
        for(int i = 0; i!= 4; i++)
        {
            colour_defined[0][i].append("0");
        }
    }

    QFile Colour_saved_2(Project_Name + "/Colour_Set_temp2.txt");
    if(Colour_saved_2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in2(&Colour_saved_2);
        for(int j = 0; j != 4; j++)
        {
            in2 >> Colour_Data[1][j][0][0] >> Colour_Data[1][j][0][1];
            in2 >> Colour_Data[1][j][1][0] >> Colour_Data[1][j][1][1];
            in2 >> Colour_Data[1][j][2][0] >> Colour_Data[1][j][2][1];
            in2 >> Colour_Data[1][j][3][0] >> Colour_Data[1][j][3][1];
            in2 >> Colour_Data[1][j][4][0] >> Colour_Data[1][j][4][1];
            in2 >> Colour_Data[1][j][5][0] >> Colour_Data[1][j][5][1];
            in2 >> Colour_Data[1][j][6][0] >> Colour_Data[1][j][6][1];
            in2 >> Colour_Data[1][j][7][0] >> Colour_Data[1][j][7][1];
            in2 >> Colour_Data[1][j][8][0] >> Colour_Data[1][j][8][1];
            in2 >> Colour_Data[1][j][9][0] >> Colour_Data[1][j][9][1];
            in2 >> Colour_Data[1][j][10][0] >> Colour_Data[1][j][10][1];
            in2 >> Colour_Data[1][j][11][0] >> Colour_Data[1][j][11][1];
            in2 >> Colour_Data[1][j][12][0] >> Colour_Data[1][j][12][1];
            in2 >> Colour_Data[1][j][13][0] >> Colour_Data[1][j][13][1];
            in2 >> Colour_Data[1][j][14][0] >> Colour_Data[1][j][14][1];
            in2 >> Colour_Data[1][j][15][0] >> Colour_Data[1][j][15][1];
            in2 >> Colour_Data[1][j][16][0] >> Colour_Data[1][j][16][1];
            in2 >> Colour_Data[1][j][17][0] >> Colour_Data[1][j][17][1];
            in2 >> Colour_Data[1][j][18][0] >> Colour_Data[1][j][18][1];
            in2 >> Colour_Data[1][j][19][0] >> Colour_Data[1][j][19][1];
            in2 >> Colour_Data[1][j][20][0] >> Colour_Data[1][j][20][1];
            in2 >> Colour_Data[1][j][21][0] >> Colour_Data[1][j][21][1];
            in2 >> Colour_Data[1][j][22][0] >> Colour_Data[1][j][22][1];
            in2 >> Colour_Data[1][j][23][0] >> Colour_Data[1][j][23][1];
            in2 >> Colour_Data[1][j][24][0] >> Colour_Data[1][j][24][1];
            in2 >> Colour_Data[1][j][25][0] >> Colour_Data[1][j][25][1];
            in2 >> Colour_Data[1][j][26][0] >> Colour_Data[1][j][26][1];
            in2 >> Colour_Data[1][j][27][0] >> Colour_Data[1][j][27][1];
            in2 >> Colour_Data[1][j][28][0] >> Colour_Data[1][j][28][1];
        }
        Colour_saved_2.close();
    }
    else
    {
        for(int i = 0; i!= 4; i++)
        {
            colour_defined[1][i].append("0");
        }
    }

    for(int i = 0; i != 2; i++)
    {
        for(int j = 0; j != 4; j++)
        {
            for(int k = 0; k != 29; k++)
            {
                if(Colour_Data[i][j][k][0] != 0 || Colour_Data[i][j][k][1] != 0)
                {
                    if(Colour_Data[i][j][k][0] != 0 && Colour_Data[i][j][k][1] != 0)
                    {
                        if(colour_defined[i][j].right(1) != " ")
                        {
                            colour_defined[i][j].append(" + ");
                        }
                        if(Colour_Data[i][j][k][1] > 0)
                        {
                            colour_defined[i][j].append(tr("(") +
                                QString::number(Colour_Data[i][j][k][0]) +
                                "t + " +
                                QString::number(Colour_Data[i][j][k][1]) +
                                ")" +
                                Formula[i][k]);
                        }
                        else
                        {
                            colour_defined[i][j].append(tr("(") +
                                QString::number(Colour_Data[i][j][k][0]) +
                                "t - " +
                                QString::number(-Colour_Data[i][j][k][1]) +
                                ")" +
                                Formula[i][k]);
                        }
                    }
                    else if(Colour_Data[i][j][k][0] != 0)
                    {
                        if(Colour_Data[i][j][k][0] > 0)
                        {
                            if(colour_defined[i][j].right(1) != " ")
                            {
                                colour_defined[i][j].append(" + ");
                            }
                            colour_defined[i][j].append(QString::number(Colour_Data[i][j][k][0]) + "t " + Formula[i][k]);
                        }
                        else
                        {
                            colour_defined[i][j].append(" - ");
                            colour_defined[i][j].append(QString::number(-Colour_Data[i][j][k][0]) + "t " + Formula[i][k]);
                        }
                    }
                    else
                    {
                        if(Colour_Data[i][j][k][1] > 0)
                        {
                            if(colour_defined[i][j].right(1) != " ")
                            {
                                colour_defined[i][j].append(" + ");
                            }
                            colour_defined[i][j].append(QString::number(Colour_Data[i][j][k][1]) + " " + Formula[i][k]);
                        }
                        else
                        {
                            colour_defined[i][j].append(" - ");
                            colour_defined[i][j].append(QString::number(-Colour_Data[i][j][k][1]) + " " + Formula[i][k]);
                        }
                    }
                    if(k == 28) colour_defined[i][j].chop(1);
                }
            }
        }
    }
    for(int i = 0; i != 2; i++)
    {
        for(int j = 0; j != 4; j++)
        {
            if(colour_defined[i][j].right(1) == " ")
            {
                colour_defined[i][j].append("0");
            }
        }
    }

    ui->Convergent_Points_Colour_Formula_View->setTextColor(qRgb(236, 28, 36)); // red
    ui->Convergent_Points_Colour_Formula_View->setText(colour_defined[0][0]);
    ui->Convergent_Points_Colour_Formula_View->setTextColor(qRgb(25, 193, 72)); // green
    ui->Convergent_Points_Colour_Formula_View->append(colour_defined[0][1]);
    ui->Convergent_Points_Colour_Formula_View->setTextColor(Qt::blue);
    ui->Convergent_Points_Colour_Formula_View->append(colour_defined[0][2]);
    ui->Convergent_Points_Colour_Formula_View->setTextColor(Qt::gray);
    ui->Convergent_Points_Colour_Formula_View->append(colour_defined[0][3]);

    ui->Unconvergent_Points_Colour_Formula_View->setTextColor(qRgb(236, 28, 36)); // red
    ui->Unconvergent_Points_Colour_Formula_View->setText(colour_defined[1][0]);
    ui->Unconvergent_Points_Colour_Formula_View->setTextColor(qRgb(25, 193, 72)); // green
    ui->Unconvergent_Points_Colour_Formula_View->append(colour_defined[1][1]);
    ui->Unconvergent_Points_Colour_Formula_View->setTextColor(Qt::blue);
    ui->Unconvergent_Points_Colour_Formula_View->append(colour_defined[1][2]);
    ui->Unconvergent_Points_Colour_Formula_View->setTextColor(Qt::gray);
    ui->Unconvergent_Points_Colour_Formula_View->append(colour_defined[1][3]);

    if(Open_Location != "" && Project_Name == "Unsaved project")
    {
        Version_Higher_Than_4 = true;
    }
}

void MainWindow::edit(int mode) // default as EDIT_HERE
{
    if(NO_EDIT) return;

    HIGH_V_ONLY
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
        if(Project_Template == "1") curr_info.template_ = 1;
        if(Project_Template == "2") curr_info.template_ = 2;
        if(Project_Template == "3") curr_info.template_ = 3;
        if(Project_Template == "4") curr_info.template_ = 4;

        // Image Value
        curr_info.min_class_v = ui->Min_class_value->value();
        curr_info.max_class_v = ui->Max_class_value->value();
        curr_info.max_loop_t  = ui->Max_loop_time->value();
/*
        // Colour 1
        For_All_Colour(i, j)
            curr_info.Colour_Data_1[i][j][0] = Colour_Data_S[0][i][j][0];
            curr_info.Colour_Data_1[i][j][1] = Colour_Data_S[0][i][j][1];
        End_All_Colour

        // Colour 2
        For_All_Colour(i, j)
            curr_info.Colour_Data_2[i][j][0] = Colour_Data_S[1][i][j][0];
            curr_info.Colour_Data_2[i][j][1] = Colour_Data_S[1][i][j][1];
        End_All_Colour
*/
        // Route Info
        QList<Route_Info> Route;
        for(int i = 0; i != model->rowCount(); i++)
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

        // Video Info
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
    HIGH_V_ONLY

    NO_EDIT = true;

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
    default:
        // uncheck all
        ui->Template_Choice_1->setAutoExclusive(false);
        ui->Template_Choice_2->setAutoExclusive(false);
        ui->Template_Choice_3->setAutoExclusive(false);
        ui->Template_Choice_4->setAutoExclusive(false);
        ui->Template_Choice_1->setChecked(false);
        ui->Template_Choice_2->setChecked(false);
        ui->Template_Choice_3->setChecked(false);
        ui->Template_Choice_4->setChecked(false);
        ui->Template_Choice_1->setAutoExclusive(true);
        ui->Template_Choice_2->setAutoExclusive(true);
        ui->Template_Choice_3->setAutoExclusive(true);
        ui->Template_Choice_4->setAutoExclusive(true);
        Project_Template = "Undefined";
        break;
    }

    // Image Value
    ui->Min_class_value->setValue(curr_info.min_class_v);
    ui->Max_class_value->setValue(curr_info.max_class_v);
    ui->Max_loop_time->setValue(curr_info.max_loop_t);

    // Colour
    // It is initialized in class Set_Colour itself using curr_info.
    showColourFormula();

    // Route Info
    isRouteValid = true;
    int preRowCount = model->rowCount();
    model->setRowCount(curr_info.Route_.size());
    for(int i = preRowCount; i < curr_info.Route_.size(); i++)
    {
        for(int j = 0; j != 6; j++)
        {
            model->setItem(i, j, new QStandardItem("0"));
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
            model->item(0, 0)->setForeground(QBrush(Qt::red));
            model->item(0, 0)->setToolTip("The first t should be 0.");
            isRouteValid = false;
        }
        else if(i == curr_info.Route_.size() - 1 && Tb(i, 0) != 1)
        {
            model->item(i, 0)->setForeground(QBrush(Qt::red));
            model->item(i, 0)->setToolTip("The last t should be 1.");
            isRouteValid = false;
        }
        else if(i != 0 && Tb(i - 1, 0) > Tb(i, 0))
        {
            model->item(i, 0)->setForeground(QBrush(Qt::red));
            model->item(i, 0)->setToolTip("It is not in order.");
            isRouteValid = false;
        }
        else
        {
            model->item(i, 0)->setForeground(QBrush(Qt::black));
            model->item(i, 0)->setToolTip("");
        }

        if(Tb(i, 4) <= 0)
        {
            model->item(i, 4)->setForeground(QBrush(Qt::red));
            model->item(i, 4)->setToolTip("It should be larger than 0");
            isRouteValid = false;
        }
        else
        {
            model->item(i, 4)->setForeground(QBrush(Qt::black));
            model->item(i, 4)->setToolTip("");
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

    // Video Info
    ui->lineEdit_videoPath->setText(curr_info.video_path);
    ui->lineEdit_videoName->setText(curr_info.video_name);
     ui->textBrowser_music->clear();
    for(const auto& c : curr_info.music_list)
    {
        ui->textBrowser_music->append(c);
    }

    NO_EDIT = false;
}

void MainWindow::on_actionUndo_triggered()
{
    HIGH_V_ONLY

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
    HIGH_V_ONLY

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

void MainWindow::on_actionFRD_H_Project_triggered()
{
    LOW_V_ONLY

    Version_Higher_Than_4 = true;
    save_or_not = false;
    edit();
    on_actionSave_S_triggered();
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

void MainWindow::routeEdit(QStandardItem* it)
{
    HIGH_V_ONLY

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
    for(int i = 0; i != model->rowCount(); i++)
    {
        if(!model->item(i, 5)) return;
        Route_Info temp_info {Tb(i, 0), Tb(i, 1), Tb(i, 2), Tb(i, 3), Tb(i, 4), Tb(i, 5)};
        model_list.push_back(temp_info);
    }
    if(model_list != curr_info.Route_)
    {
        qDebug() << "routeEdit";
        // curr_info.setRouteInfo(model_list);
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
    HIGH_V_ONLY
    if(ui->Min_class_value->value() == curr_info.min_class_v) return;
    edit();
}

void MainWindow::on_Max_class_value_editingFinished()
{
    HIGH_V_ONLY
    if(ui->Max_class_value->value() == curr_info.max_class_v) return;
    edit();
}

void MainWindow::on_Max_loop_time_editingFinished()
{
    HIGH_V_ONLY
    if(ui->Max_loop_time->value() == curr_info.max_loop_t) return;
    edit();
}

void MainWindow::showColourFormula()
{
    HIGH_V_ONLY

    QString Formula[2][29] = {
        {
            "k",
            "Zn.X",
            "Zn.Y",
            "|Zn|",
            "|Zn| / MIN",
            "argz(Zn)",
            "sin(argz(Zn))",
            "cos(argz(Zn))",
            "Z0.X",
            "Z0.Y",
            "|Z0|",
            "|Z0| / MIN",
            "argz(Z0)",
            "sin(argz(Z0))",
            "|Zn-Z0|",
            "e^k",
            "10^k",
            "ln(1+|Zn|)",
            "ln(1+|Z0|)",
            "ln(1+|Zn-Z0|)",
            "e^|Zn|",
            "e^|Z0|",
            "e^|Zn-Z0|",
            "10^|Zn|",
            "10^|Z0|",
            "10^|Zn-Z0|",
            "e^(|Zn| / MIN)",
            "e^(|Z0| / MIN)",
            ""
        },
        {
            "k",
            "Zn.X",
            "Zn.Y",
            "|Zn|",
            "|Zn| / MAX",
            "argz(Zn)",
            "sin(argz(Zn))",
            "cos(argz(Zn))",
            "Z0.X",
            "Z0.Y",
            "|Z0|",
            "|Z0| / MAX",
            "argz(Z0)",
            "sin(argz(Z0))",
            "|Zn-Z0|",
            "e^k",
            "10^k",
            "ln(1+|Zn|)",
            "ln(1+|Z0|)",
            "ln(1+|Zn-Z0|)",
            "e^|Zn|",
            "e^|Z0|",
            "e^|Zn-Z0|",
            "10^|Zn|",
            "10^|Z0|",
            "10^|Zn-Z0|",
            "e^(|Zn| / MAX)",
            "e^(|Z0| / MAX)",
            ""
        }
    };

    QString colour_defined[2][4] = {
        {"R = ", "G = ", "B = ", "A = "},
        {"R = ", "G = ", "B = ", "A = "}
    };
    //double Colour_SET[2][4][29][2] = {0};
    //Colour_Data[0][3][28][1] = Colour_Data[1][3][28][1] = 255;

#ifndef Colour_SET
#define Colour_SET(i) ((!i) ? curr_info.Colour_Data_1 : curr_info.Colour_Data_2)

    for(int i = 0; i != 2; i++)
    {
        for(int j = 0; j != 4; j++)
        {
            for(int k = 0; k != 29; k++)
            {
                if(Colour_SET(i)[j][k][0] != 0 || Colour_SET(i)[j][k][1] != 0)
                {
                    if(Colour_SET(i)[j][k][0] != 0 && Colour_SET(i)[j][k][1] != 0)
                    {
                        if(colour_defined[i][j].right(1) != " ")
                        {
                            colour_defined[i][j].append(" + ");
                        }
                        if(Colour_SET(i)[j][k][1] > 0)
                        {
                            colour_defined[i][j].append(tr("(") +
                                QString::number(Colour_SET(i)[j][k][0]) +
                                "t + " +
                                QString::number(Colour_SET(i)[j][k][1]) +
                                ")" +
                                Formula[i][k]);
                        }
                        else
                        {
                            colour_defined[i][j].append(tr("(") +
                                QString::number(Colour_SET(i)[j][k][0]) +
                                "t - " +
                                QString::number(-Colour_SET(i)[j][k][1]) +
                                ")" +
                                Formula[i][k]);
                        }
                    }
                    else if(Colour_SET(i)[j][k][0] != 0)
                    {
                        if(Colour_SET(i)[j][k][0] > 0)
                        {
                            if(colour_defined[i][j].right(1) != " ")
                            {
                                colour_defined[i][j].append(" + ");
                            }
                            colour_defined[i][j].append(QString::number(Colour_SET(i)[j][k][0]) + "t " + Formula[i][k]);
                        }
                        else
                        {
                            colour_defined[i][j].append(" - ");
                            colour_defined[i][j].append(QString::number(-Colour_SET(i)[j][k][0]) + "t " + Formula[i][k]);
                        }
                    }
                    else
                    {
                        if(Colour_SET(i)[j][k][1] > 0)
                        {
                            if(colour_defined[i][j].right(1) != " ")
                            {
                                colour_defined[i][j].append(" + ");
                            }
                            colour_defined[i][j].append(QString::number(Colour_SET(i)[j][k][1]) + " " + Formula[i][k]);
                        }
                        else
                        {
                            colour_defined[i][j].append(" - ");
                            colour_defined[i][j].append(QString::number(-Colour_SET(i)[j][k][1]) + " " + Formula[i][k]);
                        }
                    }
                    if(k == 28) colour_defined[i][j].chop(1);
                }
            }
        }
    }
    for(int i = 0; i != 2; i++)
    {
        for(int j = 0; j != 4; j++)
        {
            if(colour_defined[i][j].right(1) == " ")
            {
                colour_defined[i][j].append("0");
            }
        }
    }

#undef Colour_SET
#endif

    ui->Convergent_Points_Colour_Formula_View->setTextColor(qRgb(236, 28, 36)); // red
    ui->Convergent_Points_Colour_Formula_View->setText(colour_defined[0][0]);
    ui->Convergent_Points_Colour_Formula_View->setTextColor(qRgb(25, 193, 72)); // green
    ui->Convergent_Points_Colour_Formula_View->append(colour_defined[0][1]);
    ui->Convergent_Points_Colour_Formula_View->setTextColor(Qt::blue);
    ui->Convergent_Points_Colour_Formula_View->append(colour_defined[0][2]);
    ui->Convergent_Points_Colour_Formula_View->setTextColor(Qt::gray);
    ui->Convergent_Points_Colour_Formula_View->append(colour_defined[0][3]);

    ui->Unconvergent_Points_Colour_Formula_View->setTextColor(qRgb(236, 28, 36)); // red
    ui->Unconvergent_Points_Colour_Formula_View->setText(colour_defined[1][0]);
    ui->Unconvergent_Points_Colour_Formula_View->setTextColor(qRgb(25, 193, 72)); // green
    ui->Unconvergent_Points_Colour_Formula_View->append(colour_defined[1][1]);
    ui->Unconvergent_Points_Colour_Formula_View->setTextColor(Qt::blue);
    ui->Unconvergent_Points_Colour_Formula_View->append(colour_defined[1][2]);
    ui->Unconvergent_Points_Colour_Formula_View->setTextColor(Qt::gray);
    ui->Unconvergent_Points_Colour_Formula_View->append(colour_defined[1][3]);
}

void MainWindow::dealRouteSort(int)
{
    HIGH_V_ONLY
    edit();
    display(); // used to check the validity
}

void MainWindow::on_Image_size_X_editingFinished()
{
    HIGH_V_ONLY
    if(ui->Image_size_X->value() == curr_info.image_size_x) return;
    edit();
}

void MainWindow::on_Image_size_Y_editingFinished()
{
    HIGH_V_ONLY
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
    HIGH_V_ONLY
    if(ui->timeEdit->time().toString("mmss") == curr_info.total_time_str) return;
    edit();
}

void MainWindow::on_lineEdit_imagePath_editingFinished()
{
    HIGH_V_ONLY
    if(ui->lineEdit_imagePath->text() == curr_info.img_path) return;
    edit();
}

void MainWindow::on_lineEdit_imagePrefix_editingFinished()
{
    HIGH_V_ONLY
    if(ui->lineEdit_imagePrefix->text() == curr_info.img_prefix) return;
    edit();
}

void MainWindow::on_lineEdit_videoPath_editingFinished()
{
    HIGH_V_ONLY
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
    QDesktopServices::openUrl(QUrl("https://blog.csdn.net/weixin_50012998/article/details/114821632"));
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
    HIGH_V_ONLY
    switch(curr_info.template_)
    {
    case 1:
        if(curr_info.min_class_v == 0.25) return;
        ui->Min_class_value->setValue(0.25);
        edit();
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_Max_value_default_clicked()
{
    HIGH_V_ONLY
    switch(curr_info.template_)
    {
    case 1:
        if(curr_info.max_class_v == 2) return;
        ui->Max_class_value->setValue(2);
        edit();
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_Max_loop_default_clicked()
{
    HIGH_V_ONLY
    switch(curr_info.template_)
    {
    case 1:
        if(curr_info.max_loop_t == 50) return;
        ui->Max_loop_time->setValue(50);
        edit();
        break;
    default:
        break;
    }
}

void MainWindow::on_pushButton_IV_clear1_clicked()
{
    HIGH_V_ONLY
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
    HIGH_V_ONLY
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
    HIGH_V_ONLY
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

void MainWindow::on_actionFRD_4_Help_triggered()
{
    FRD_4_Help* FRD_4_Help_Window = new FRD_4_Help(this);
    FRD_4_Help_Window->show();
}

void MainWindow::NewFeatures()
{
    on_actionNew_Features_triggered();
}

void MainWindow::on_actionClose_triggered()
{
    HIGH_V_ONLY

    if(save_or_not) on_actionSave_S_triggered();
    setWindowTitle("Fractal Designer - Unsaved Project");
    Open_Location = "";
    Info_Save new_info;
    current_info_v = 0;
    redo_max_depth = 1;
    Project_Template = "Undefined";
    curr_info = new_info;
    save_or_not = false;
    display();

    ui->actionClose->setDisabled(true);
    Version_Higher_Than_4 = true;
}

void MainWindow::on_commandLinkButton_3_clicked()
{
    on_actionCreate_Video_triggered();
}

void MainWindow::on_actionCreate_Images_in_Range_triggered()
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

    if((!Version_Higher_Than_4 && (Project_Template == "" || Project_Template == "Undefined")) || (Version_Higher_Than_4 && Open_Location == ""))
    {
        QMessageBox::warning(this, "Can not create images", "You have not chosen a template.");
        return;
    }

    if(Version_Higher_Than_4 && !isRouteValid)
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
        while(current_index + 2 < model->rowCount() && T > Tb(current_index + 1, 0))
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
        Create_Images_Task_Pre(create_images);
        if(i != to_i)
        {
            create_images->setImage(x, y, width, width * Y / X, X, Y, angle, T, image_format, path, name + QString::number(i), "Create_Image");
        }
        else
        {
            create_images->setImage(x, y, width, width * Y / X, X, Y, angle, T, image_format, path, name + QString::number(i), "Create_Image_Last");
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
        while(current_index + 2 < model->rowCount() && T > Tb(current_index + 1, 0))
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
        Create_Images_Task_Pre(create_images);

        // qDebug() << t << current_index << x << y << width << angle;
        if(i != *(--list.end()))
        {
            create_images->setImage(x, y, width, width * Y / X, X, Y, angle, T, image_format, path, name + QString::number(i), "Create_Image");
        }
        else
        {
            create_images->setImage(x, y, width, width * Y / X, X, Y, angle, T, image_format, path, name + QString::number(i), "Create_Image_Last");
        }
        QThreadPool::globalInstance()->start(create_images);
    }
}

void MainWindow::on_actionVersion_3_triggered()
{
    QDesktopServices::openUrl(QUrl("https://www.bilibili.com/video/BV17K4y1J7XM"));
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

            if((!Version_Higher_Than_4 && (Project_Template == "" || Project_Template == "Undefined")) || (Version_Higher_Than_4 && Open_Location == ""))
            {
                QMessageBox::warning(this, "Can not create images", "You have not chosen a template.");
                return;
            }

            if(Version_Higher_Than_4 && !isRouteValid)
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

            if((!Version_Higher_Than_4 && (Project_Template == "" || Project_Template == "Undefined")) || (Version_Higher_Than_4 && Open_Location == ""))
            {
                QMessageBox::warning(this, "Can not create images", "You have not chosen a template.");
                return;
            }

            if(Version_Higher_Than_4 && !isRouteValid)
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
