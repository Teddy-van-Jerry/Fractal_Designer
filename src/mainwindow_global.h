#ifndef MAINWINDOW_GLOBAL_H
#define MAINWINDOW_GLOBAL_H

#include <QMainWindow>
#include <QMenu>
#include <QRect>
#include <QPoint>
#include <QCursor>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMetaMethod>
#include <QMetaObject>
#include <QDockWidget>
#include <QMainWindow>
#include <QMouseEvent>
#include <QVBoxLayout>
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
#include <QLayout>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDialog>
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
#include "FRD_Editor.h"
#include "FRD_TitleBar.h"
#include "Interpreter.h"
#include "Info.h"

#define OPEN_FILE_IN  0
#define OPEN_FILE_OUT 1
#define _MAX_SAVE_    50
#define curr_info     pre_info[current_info_v]
#define EDIT_HERE     0
#define EDIT_ALREADY  1

#define Tb(i, j)       table_route_model->item(i, j)->data(Qt::EditRole).value<double>()
#define SetTb(i, j, v) table_route_model->item(i, j)->setData(QVariant(v), Qt::EditRole)

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

#endif // MAINWINDOW_GLOBAL_H
