#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include <QLineEdit>
#include <QThread>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QDir>
#include <QFileDialog>
#include <QScrollArea>
#include <QProcess>
#include <QIODeviceBase>
#include <QByteArray>
#include <QItemEditorFactory>
#include <QDoubleSpinBox>
#include <QStyledItemDelegate>
#include <QButtonGroup>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>
#include <complex> // std::complex
#include "login.h"
#include "new_file.h"
#include "route_tool.h"
#include "create_image_info.h"
#include "search_result.h"
#include "info_save.h"
#include "route_info.h"
#include "version.h"
#include "new_features.h"
#include "create_images_range.h"
#include "create_image_task.h"
#include "template_2_settings.h"
#include "template_4_settings.h"
#include "preview_setting.h"

#define OPEN_FILE_IN  0
#define OPEN_FILE_OUT 1
#define _MAX_SAVE_    50
#define curr_info     pre_info[current_info_v]
#define EDIT_HERE     0
#define EDIT_ALREADY  1

#define Tb(i, j)       model->item(i, j)->data(Qt::EditRole).value<double>()
#define SetTb(i, j, v) model->item(i, j)->setData(QVariant(v), Qt::EditRole)

#define NameIs(C1_, C2_) (name_1 == C1_ && name_2 == C2_)

#ifndef For_All_Colour
#define For_All_Colour(it__, jt__) \
    for(int it__ = 0; it__ != 4; it__++) { \
        for(int jt__ = 0; jt__ != 29; jt__++) {
#endif

#ifndef End_All_Colour
#define End_All_Colour }}
#endif

#ifndef UNSUPPORTED_PLATFORM
#define UNSUPPORTED_PLATFORM \
    QMessageBox::critical(this, "Error", "This project now only supports Windows and Linux.")
#endif

const QString Chinese_Help_Url = "https://frd.teddy-van-jerry.org/help/fractal-designer-5-6-lts-help-zh";
const QString English_Help_Url = "https://frd.teddy-van-jerry.org/help/fractal-designer-5-6-lts-help";
const QString Bug_Report       = "https://github.com/Teddy-van-Jerry/Fractal_Designer/issues/11";

class PeciseDoubleFactory : public QItemEditorFactory
{
public:
    PeciseDoubleFactory() = default;
    virtual ~PeciseDoubleFactory() = default;
    virtual QWidget* createEditor(int userType, QWidget *parent) const override
    {
        if(userType == QVariant::Double)
        {
#ifndef DBL_MAX
#define DBL_MAX 1E10
#endif
            QDoubleSpinBox *sb = new QDoubleSpinBox(parent);
            sb->setFrame(false);
            sb->setMinimum(-DBL_MAX);
            sb->setMaximum(DBL_MAX);
            sb->setDecimals(11); // This is the maximum number of decimals you want
            sb->setSingleStep(0.1);
            // sb->interpretText();
            return sb;
#undef DBL_MAX
        }
        return QItemEditorFactory::createEditor(userType, parent);
    }
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct Route_info;
struct info;
class Info_Save;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    friend class Search_Result;
    friend class Route_Tool;
    friend class Info_Save;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool isRouteValid = false;

    bool NO_EDIT = false;

    uint8_t FRD_Version[4] = {5, 6, 3, 3};

    QString Open_Location = "";

    QString Project_Name;

    QString Project_Template;

    QString User_Name;

    QStandardItemModel* model = new QStandardItemModel();

    Info_Save pre_info[_MAX_SAVE_], buff_info;

    int current_info_v = 0;

    int redo_max_depth = 0;

    int from_i = 0, to_i = -1;

    void setOpenLocation(QString);

    bool High_Version_Open(int type = 0);

    bool save_or_not = false;

    void show_template_graph(); //

    void show_preview_image(); //

    void edit(int mode = EDIT_HERE);

    void display();

    bool isColourAutoSaved();

    void saveElsewhere();

    void Version_Dialog_Support(int index);

    void NewFeatures();

    void createImagesInList(const QList<int>& list);

    void createImagesInRange(int From_I, int To_I);

    bool existImage(int) const;

    void deleteImage(int);

    bool createImagePre(Create_Image_Task* task);

    void customTemplatePre(Create_Image_Task* task);

    std::complex<double> _curr_complex(const std::complex<double>& c1, const std::complex<double>& c2, double t, double k = 0);

public slots:

    void getImage(QImage img);

    void updateProgressBar(double p);

    void build_image_finished_deal();

    void build_image_one_ok();

private slots:

    void getUserName(QString);

    void on_Button_Login_MainWindow_clicked();

    void on_Button_Search_clicked();

    void on_MainWindow_exit_clicked();

    void on_actionExit_E_triggered();

    void on_actionChinese_triggered();

    void on_MainWindow_AboutTVJ_clicked();

    void on_pushButton_Chinese_Help_clicked();

    void on_actionNew_N_triggered();

    void on_MainWindow_Newfile_clicked();

    void on_actionOpen_O_triggered();

    void on_MainWindow_openfile_clicked();

    void on_actionRoute_Tool_triggered();

    void resizeEvent(QResizeEvent *Event);

    void on_Tab_currentChanged(int index);

    void on_actionSave_S_triggered();

    void on_Template_Choice_1_toggled(bool checked);

    void on_Template_Choice_2_toggled(bool checked);

    void on_Template_Choice_3_toggled(bool checked);

    void on_Template_Choice_4_toggled(bool checked);

    void on_Template_Choice_5_toggled(bool checked);

    void on_actionPreview_Refresh_triggered();

    void dealClose(QObject* sd);

    void closeEvent(QCloseEvent* Event);

    void iniTemplate(int n);

    void iniValue(double, double, int);

    void iniSize(int, int);

    void iniRouteInfo(QVector<double>);

    void iniImagePath(QString);

    void iniImagePrefix(QString);

    void iniFrameRate(int);

    void iniTotalTime(QString);

    void iniMusicAdded(QVector<QString>);

    void on_Slider_t_valueChanged(int value);

    void on_doubleSpinBox_t_editingFinished();

    void on_pushButton_routeAdd_clicked();

    void on_pushButton_routeRemove_clicked();

    void on_pushButton_routeClear_clicked();

    void on_actionCreate_Images_triggered();

    void on_commandLinkButton_Image_clicked();

    void on_Slider_t_sliderReleased();

    void on_actionStop_triggered();

    void on_toolButton_imagePath_clicked();

    void on_actionCreate_Video_triggered();

    void on_toolButton_videoPath_clicked();

    void on_actionEnglish_triggered();

    void on_MainWindow_HelpEnglish_clicked();

    void on_pushButton_addMusic_clicked();

    void on_pushButton_deleteMusic_clicked();

    void on_pushButton_UniformMotion_clicked();

    void on_actionFFmpeg_triggered();

    void on_actionCheck_Update_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_Template_Choice_1_clicked();

    void on_Template_Choice_2_clicked();

    void on_Template_Choice_3_clicked();

    void on_Template_Choice_4_clicked();

    void on_Template_Choice_5_clicked();

    void routeEdit(QStandardItem*);

    void on_Min_class_value_editingFinished();

    void on_Max_class_value_editingFinished();

    void on_Max_loop_time_editingFinished();

    void dealRouteSort(int);

    void on_Image_size_X_editingFinished();

    void on_Image_size_Y_editingFinished();

    void on_comboBox_fps_currentIndexChanged(int index);

    void on_timeEdit_editingFinished();

    void on_lineEdit_imagePath_editingFinished();

    void on_lineEdit_imagePrefix_editingFinished();

    void on_lineEdit_videoPath_editingFinished();

    void on_lineEdit_videoName_editingFinished();

    void on_actionVersion_1_triggered();

    void on_actionAbout_Teddy_van_Jerry_triggered();

    void on_actionVersion_2_triggered();

    void on_actionBug_Report_triggered();

    void on_actionVersion_triggered();

    void on_actionNew_Features_triggered();

    void on_pushButton_Min_class_default_clicked();

    void on_pushButton_Max_value_default_clicked();

    void on_pushButton_Max_loop_default_clicked();

    void on_pushButton_IV_clear1_clicked();

    void on_pushButton_IV_clear2_clicked();

    void on_pushButton_IV_clear3_clicked();

    void on_actionClose_triggered();

    void on_commandLinkButton_3_clicked();

    void on_actionCreate_Images_in_Range_triggered();

    void on_actionVersion_3_triggered();

    void on_actionVersion_4_triggered();

    void on_actionGitHub_Repository_triggered();

    void on_actionCheck_Images_triggered();

    void on_actionDelete_Images_triggered();

    void on_actionMost_triggered();

    void on_actionMore_triggered();

    void on_actionNormal_triggered();

    void on_actionLess_triggered();

    void on_actionLeast_triggered();

    void on_actionTemplate_2_triggered();

    void on_actionTemplate_6_triggered();

    void on_actionVersion_5_triggered();

    void on_actionVersion_6_triggered();

    void on_MainWindow_AboutFD_clicked();

    void on_actionEnglish_2_triggered();

    void on_actionChinese_2_triggered();

    void on_Convergent_Points_Colour_Formula_textChanged();

    void on_Divergent_Points_Colour_Formula_textChanged();

    void on_lineEdit_Custom_Formula_editingFinished();

    void on_actionPreview_Settings_triggered();

    void on_actionReset_Colour_Definition_triggered();

    void on_checkBox_yInverse_stateChanged(int arg1);

    void on_pushButton_Template_Help_clicked();

    void on_actionTheme_Light_triggered();

    void on_actionTheme_Amoled_triggered();

    void on_actionTheme_Aqua_triggered();

    void on_actionTheme_Console_triggered();

    void on_actionTheme_Elegant_triggered();

    void on_actionTheme_Macos_triggered();

    void on_actionTheme_ManjaroMix_triggered();

    void on_actionTheme_MaterialDark_triggered();

    void on_actionTheme_Ubuntu_triggered();

signals:

    void Search_clicked(QString);

    void build_signal(double x, double y, double x_width, double y_height, int X, int Y, double rotate_angle, double t,
                      QString img_format, QString img_path, QString img_title, QString work_name);

    void pathShare(QString);

    void InfoForColourSet_signal(QString);

    void build_image_info_signal(QString name, QString format, int total, int start);

    void build_image_info_signal_(QString name, QString format, QList<int> list);

    void build_image_updateInfo_signal();

    void shareVersion(bool);

    void createImageStop();

private:
    void useDarkIcon();
    void useWhiteIcon();
    void ReadInit(const QString& key, QString &value);
    void WriteInit(const QString& key, const QString& value);
    void ReadStyle();

private:
    Ui::MainWindow *ui;

    QLineEdit* Line_Search;

    QToolButton* Button_Search;

    Search_Result* search_result;

    QLabel* Label_User_Name;

    QPushButton* Button_Login_MainWindow;

    QImage image_preview, image_T1, image_T2, image_T3, image_T4;

    Route_info* route_info;

    Create_Image_Info* create_image_info;

    PeciseDoubleFactory m_factory;

    QTranslator *translator;

    Template_2_Settings* template_2_dialog = new Template_2_Settings(this);

    Template_4_Settings* template_4_dialog = new Template_4_Settings(this);

    Preview_Setting* preview_setting = new Preview_Setting(this);

public:

    Route_Tool* route_tool_window;

    enum App_Language { LANGUAGE_ENGLISH, LANGUAGE_CHINESE } app_language = LANGUAGE_ENGLISH;

    void setLanguage(App_Language la);

    bool language_setting_no_change_now = false;

};

#endif // MAINWINDOW_H
