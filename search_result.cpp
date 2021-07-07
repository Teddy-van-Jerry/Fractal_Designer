#include "search_result.h"
#include "ui_search_result.h"

#define Btn_Con(case_number__, action_triggered__) case case_number__: \
    connect(new_btn, SIGNAL(clicked()), parent(), SLOT(action_triggered__)); \
    break

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
    for(auto& i : btn) delete i;
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
    Possible_Result All_P[29];
    All_P[0]  = {0,  "Create Images", "Action to create images based on the parameters you have set.", {"create", "image", "images", "生成", "图片", "生成图片"}};
    All_P[1]  = {1,  "Create Images in Range", "Action to create images in a specific range you set.", {"create", "image", "images", "range", "生成", "图片", "生成图片", "范围", "范围内生成图片"}};
    All_P[2]  = {2,  "Create Video", "Action to create video based on images and music if added.", {"create", "video", "生成", "视频", "生成视频"}};
    All_P[3]  = {3,  "Help (English)", "English version of help.", {"help", "english", "帮助", "查看帮助"}};
    All_P[4]  = {4,  "Help (Chinese)", "Chinese version of help. (Please make sure your computer is connected to the Internet.)", {"help", "chinese", "帮助", "查看帮助"}};
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
    All_P[19] = {19, "FRD 4 Help", "Fractal Designer 4 (Compatibility Mode) Help.", {}};
    All_P[20] = {20, "New Features", "View New Features in Fractal Designer since Version 4.7.", {"new", "feature", "features", "新特征", "新"}};
    All_P[21] = {21, "Version", "View version information.", {"version", "版本", "版本信息"}};
    All_P[22] = {22, "Bug Report", "Report the bugs you have found.", {"bug", "bugs", "report", "错误"}};
    All_P[23] = {23, "Close", "Close the project.", {"close", "关闭"}};
    All_P[24] = {24, "GitHub Repository", "View the GitHub repository of Fractal Designer.", {"update", "check", "github", "repository", "git", "查看", "最新", "版本", "最新版", "最新版本", "查看最新版", "查看最新版本", "更新", "查看更新", "仓库"}};
    All_P[25] = {25, "Check Images", "Check if images are created or missed. You can choose to recreate missed ones.", {"create", "image", "images", "check", "检查", "图片", "检查图片"}};
    All_P[26] = {26, "Delete Images", "Delete Images that have already been created.", {"delete", "image", "images", "删除", "图片", "删除图片", "删除所有图片"}};
    All_P[27] = {27, "Template 2 Settings", "Additional Settings for Template 2 (Julia Set).", {"template", "2", "setting", "settings", "set", "julia", "set", "模板", "二", "设置", "模板二设置"}};
    All_P[28] = {28, "Template 4 Settings", "Additional Settings for Template 4 (Generalized Newton Fractal).", {"template", "4", "setting", "settings", "set", "newton", "fractal", "模板", "四", "设置", "模板四设置"}};
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

    sort_possible_result(All_P, 29);

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
        Btn_Con(0 , on_actionCreate_Images_triggered());
        Btn_Con(1 , on_actionCreate_Images_in_Range_triggered());
        Btn_Con(2 , on_actionCreate_Video_triggered());
        Btn_Con(3 , on_actionEnglish_triggered());
        Btn_Con(4 , on_actionChinese_triggered());
        Btn_Con(5 , on_actionExit_E_triggered());
        Btn_Con(6 , on_MainWindow_AboutTVJ_clicked());
        Btn_Con(7 , on_actionNew_N_triggered());
        Btn_Con(8 , on_actionOpen_O_triggered());
        Btn_Con(9 , on_actionSave_S_triggered());
        Btn_Con(10, on_actionPreviewRefresh_triggered());
        Btn_Con(11, on_actionRoute_Tool_triggered());
        Btn_Con(12, on_actionCheck_Update_triggered());
        Btn_Con(13, on_actionFFmpeg_triggered());
        Btn_Con(14, on_Button_Login_MainWindow_clicked());
        Btn_Con(15, on_actionAuto_Refresh_triggered());
        Btn_Con(16, on_actionStop_triggered());
        Btn_Con(17, on_actionUndo_triggered());
        Btn_Con(18, on_actionRedo_triggered());
        // Btn_Con(19, on_actionFRD_4_Help_triggered());
        Btn_Con(20, on_actionNew_Features_triggered());
        Btn_Con(21, on_actionVersion_triggered());
        Btn_Con(22, on_actionBug_Report_triggered());
        Btn_Con(23, on_actionClose_triggered());
        Btn_Con(24, on_actionGitHub_Repository_triggered());
        Btn_Con(25, on_actionCheck_Images_triggered());
        Btn_Con(26, on_actionDelete_Images_triggered());
        Btn_Con(27, on_actionTemplate_2_triggered());
        Btn_Con(28, on_actionTemplate_6_triggered());
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
