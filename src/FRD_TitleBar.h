#ifndef FRD_TITLEBAR_H
#define FRD_TITLEBAR_H

#include <QSize>
#include <QEvent>
#include <QLabel>
#include <QPoint>
#include <QWidget>
#include <QPainter>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPushButton>
#include <QWindowStateChangeEvent>
#include <QStyleOption>
#include <stdexcept>
#include <QMenu>
#include <QToolBar>
#include <QMenuBar>
#include <QTimer>

namespace QCustomAttrs {
    enum WindowButton {
        Minimize = 0x01,
        Maximize = 0x02,
        Close    = 0x04,
        All      = Minimize | Maximize | Close
    };

    Q_DECLARE_FLAGS(WindowButtons, WindowButton)
    Q_DECLARE_OPERATORS_FOR_FLAGS(WindowButtons)
}

class FRD_TitleBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QCustomAttrs::WindowButtons windowButtons READ windowButtons WRITE setWindowButtons)
    Q_CLASSINFO("custom_obj_type", "QTitleBar")
public:
    explicit FRD_TitleBar(QWidget *parent = nullptr);
    ~FRD_TitleBar();

    void setWindowButtons(QCustomAttrs::WindowButtons btns);
    inline QCustomAttrs::WindowButtons windowButtons() const { return this->m_frameButtons; }

    void setWindowButtonEnabled(QCustomAttrs::WindowButton btn, bool enabled = true);
   // void setWindowButtonEnabled(QCustomAttrs::WindowButton btn, bool enabled = true);
    void addFRDmenu();

    void updateTitleBar();
    QMenu *FileMenu;
    QMenu *EditMenu;
    QMenu *ViewMenu;
    QMenu *TemplateMenu;
    QMenu *ImageMenu;
    QMenu *VideoMenu;
    QMenu *HelpMenu;
    QMenu *OptionsMenu;
    QMenuBar *menubar;
    QTimer *timer;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    static bool isinSide(QRect r,QPoint p);

public:
    bool canMove;
    bool maximizing;

    QPoint m_pCursor;
    const QSize FRAME_BUTTON_SIZE;

    QWidget *m_parentWindow;

    QCustomAttrs::WindowButtons m_frameButtons;

    QLabel lbl_windowTitle;
    QHBoxLayout m_layout;
    QPushButton btn_minimize;
    QPushButton btn_maximize;
    QPushButton btn_close;
    QLabel FRD_icon;

    int		borderSize = 5;

    /*---------窗口缩放----*/
    //上
    QRect	topHit;
    bool	bResizeByTopHit = false;
    //下
    QRect	bottomHit;
    bool	bResizeByBottomHit = false;
    //左
    QRect	leftHit;
    bool	bResizeByLeftHit = false;
    //右
    QRect	rightHit;
    bool	bResizeByRightHit = false;
    //右下
    QRect	rightBottomHit;
    bool	bResizeByRbHit = false;
    //右上
    QRect	rightTopHit;
    bool	bResizeByRtHit = false;
    //左下
    QRect	leftBottomHit;
    bool	bResizeByLbHit = false;
    //左上
    QRect	leftTopHit;
    bool	bResizeByLtHit = false;
signals:
    void requestClose();
    void requestMaximize();
    void requestMinimize();

private slots:
    void timeUpdate();
    void updateMaxbutton();

    void resizeEvent(QResizeEvent* e);

    // void windowStateChenged(QWindowStateChangeEvent* e);
};

#endif // FRD_TITLEBAR_H
