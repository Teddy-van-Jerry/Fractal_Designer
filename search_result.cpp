#include "search_result.h"
#include "ui_search_result.h"

Search_Result::Search_Result(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Search_Result)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
}

Search_Result::~Search_Result()
{
    delete ui;
    for(auto i : btn) delete i;
    delete label;
    delete Help_E;
    delete Help_C;
}

void sort_possible_result(Possible_Result* p, int length)
{
    for (int i = 1; i != length; i++)
    {
        Possible_Result temp = p[i];
        // slide elements right to make room for p[i]
        int j = i;
        while (j >= 1 && p[j - 1].value < temp.value)
        {
            p[j--] = p[j - 1];
        }
        p[j] = temp;
    }
}

void Search_Result::getSearch(QString search)
{
    if(already_open)
    {
        close();
        return;
    }
    else
    {
        already_open = true;
    }
    Possible_Result All_P[25];
    All_P[0]  = {0,  "Create Images", "Action to create images based on the parameters you have set.", {"create", "image", "images", "生成", "图片", "生成图片"}};
    All_P[1]  = {1,  "Create Images in Range", "Action to create images in a specific range you set.", {"create", "image", "images", "range", "生成", "图片", "生成图片", "范围", "范围内生成图片"}};
    All_P[2]  = {2,  "Create Video", "Action to create video based on images and music if added.", {"create", "video", "生成", "视频", "生成视频"}};
    All_P[3]  = {3,  "Help(English)", "English version of help.", {"help", "english", "帮助", "查看帮助"}};
    All_P[4]  = {4,  "Help(Chinese)", "Chinese version of help. (Please make sure your computer is connected to the Internet.)", {"help", "chinese", "帮助", "查看帮助"}};
    All_P[5]  = {5,  "Exit", "Exit the application.", {"exit", "close", "退出", "关闭"}};
    All_P[6]  = {6,  "About Teddy van Jerry", "View the homepage of Teddy van Jerry in CSDN.",{"about", "teddy", "van", "jerry", "tvj", "关于", "作者"}};
    All_P[7]  = {7,  "New", "Create a new project.", {"new", "新建", "新建项目"}};
    All_P[8]  = {8,  "Open", "Open an existing project.", {"open", "打开", "打开项目"}};
    All_P[9]  = {9,  "Save", "Save the project.", {"save", "保存", "保存项目"}};
    All_P[10] = {10, "Preview Refresh", "Refresh the preview image.", {"refresh", "preview", "预览", "刷新", "预览刷新", "刷新预览", "预览图刷新", "刷新预览图", "预览图片刷新", "刷新预览图片"}};
    All_P[11] = {11, "Route Tool", "Open the route tool.", {"route", "tool", "路径", "工具", "路径工具"}};
    All_P[12] = {12, "Check Update", "Check the latest version of Fractal Designer.", {"update", "check", "查看", "最新", "版本", "最新版", "最新版本", "查看最新版", "查看最新版本", "更新", "查看更新"}};
    All_P[13] = {13, "Install FFmpeg", "Install FFmpeg 4.3.1", {"install", "ffmpeg", "下载", "下载ffmpeg", "ffmpeg下载"}};
    All_P[14] = {14, "Log In", "Log in", {"log", "in", "登录"}};
    All_P[15] = {15, "Auto Refresh", "Choosing this mode enables the auto refresh of preview image when you change the value of t.", {"auto", "refresh", "preview", "自动", "刷新", "预览", "自动刷新", "自动刷新", "自动刷新预览图", "自动刷新预览图片", "预览刷新", "刷新预览", "预览图刷新", "刷新预览图", "预览图片刷新", "刷新预览图片"}};
    All_P[16] = {16, "Stop", "Stop creating images.", {"stop", "create", "image", "images", "停", "停止", "停止生成图片"}};
    All_P[17] = {17, "Undo", "Undo one step if possible.", {"undo", "撤消", "撤回"}};
    All_P[18] = {18, "Redo", "Redo one step if possible.", {"redo", "恢复"}};
    All_P[19] = {19, "FRD 4 Help", "Fractal Designer 4 (Compatibility Mode) Help.", {"help", "4", "帮助"}};
    All_P[20] = {20, "New Features", "View New Features in Fractal Designer since Version 4.7.", {"new", "feature", "features", "新特征", "新"}};
    All_P[21] = {21, "Version", "View version information.", {"version", "版本", "版本信息"}};
    All_P[22] = {22, "Bug Report", "Report the bugs you have found.", {"bug", "bugs", "report", "错误"}};
    All_P[23] = {23, "Close", "Close the project.", {"close", "关闭"}};
    All_P[24] = {24, "GitHub Repository", "View the GitHub repository of Fractal Designer.", {"update", "check", "github", "repository", "git", "查看", "最新", "版本", "最新版", "最新版本", "查看最新版", "查看最新版本", "更新", "查看更新", "仓库"}};
    QFile Button_qss(":/StyleSheet/Button_Search_Result.qss");
    Button_qss.open(QFile::ReadOnly);
    QString Button_qss_str = QLatin1String(Button_qss.readAll());
    Button_qss.close();
    search = search.toLower();
    search.replace('-', ' ');
    search.replace('_', ' ');
    search.replace(',', ' ');
    QStringList keywords = search.split(' ');
    for(auto& words : keywords)
    {
        for(auto& key_p : All_P)
        {
            for(auto& key : key_p.Key_Words)
            {
                if(words == key)
                {
                    key_p.value++;
                }
            }
        }
    }

    sort_possible_result(All_P, 25);

    //connect(btn, SIGNAL(clicked()), this, SLOT(search_result_clicked()));

    for (auto result : All_P)
    {
        qDebug() << result.value;
        if(result.value == 0) break;
        QPushButton* new_btn = new QPushButton();
        new_btn->setText(result.Text);
        new_btn->setToolTip(result.Tip);
        new_btn->setStyleSheet(Button_qss_str);
        btn.push_back(new_btn);
        switch(result.index)
        {
        case 0:  connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionCreate_Images_triggered())); break;
        case 1:  connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionCreate_Images_in_Range_triggered())); break;
        case 2:  connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionCreate_Video_triggered())); break;
        case 3:  connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionEnglish_triggered())); break;
        case 4:  connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionChinese_triggered())); break;
        case 5:  connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionExit_E_triggered())); break;
        case 6:  connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_MainWindow_AboutTVJ_clicked())); break;
        case 7:  connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionNew_N_triggered())); break;
        case 8:  connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionOpen_O_triggered())); break;
        case 9:  connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionSave_S_triggered())); break;
        case 10: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionPreviewRefresh_triggered())); break;
        case 11: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionRoute_Tool_triggered())); break;
        case 12: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionCheck_Update_triggered())); break;
        case 13: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionFFmpeg_triggered())); break;
        case 14: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_Button_Login_MainWindow_clicked())); break;
        case 15: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionAuto_Refresh_triggered())); break;
        case 16: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionStop_triggered())); break;
        case 17: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionUndo_triggered())); break;
        case 18: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionRedo_triggered())); break;
        case 19: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionFRD_4_Help_triggered())); break;
        case 20: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionNew_Features_triggered())); break;
        case 21: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionVersion_triggered())); break;
        case 22: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionBug_Report_triggered())); break;
        case 23: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionClose_triggered())); break;
        case 24: connect(new_btn, SIGNAL(clicked()), parent(), SLOT(on_actionGitHub_Repository_triggered())); break;
        default: break;
        }
        ui->gridLayout_search->addWidget(new_btn);
    }
    if(All_P[0].value == 0)
    {
        // if(label) delete label;
        label = new QLabel();
        label->setText("Sorry, no result found.\nYou can find more information in Help.");
        label->setAlignment(Qt::AlignCenter);
        ui->gridLayout_search->addWidget(label);
        Help_E = new QPushButton();
        Help_C = new QPushButton();
        Help_E->setText(All_P[3].Text);
        Help_E->setToolTip(All_P[3].Tip);
        Help_E->setStyleSheet(Button_qss_str);
        Help_C->setText(All_P[4].Text);
        Help_C->setToolTip(All_P[4].Tip);
        Help_C->setStyleSheet(Button_qss_str);
        connect(Help_E, SIGNAL(clicked()), parent(), SLOT(on_actionEnglish_triggered()));
        connect(Help_C, SIGNAL(clicked()), parent(), SLOT(on_actionChinese_triggered()));
        ui->gridLayout_search->addWidget(Help_E);
        ui->gridLayout_search->addWidget(Help_C);
    }
}
