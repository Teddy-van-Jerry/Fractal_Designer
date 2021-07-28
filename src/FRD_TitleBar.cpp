#include "FRD_TitleBar.h"

FRD_TitleBar::FRD_TitleBar(QWidget *parent) :
    QWidget(parent),FRAME_BUTTON_SIZE(24, 24)
{
    this->canMove = false;
    this->maximizing = false;
    this->m_frameButtons = QCustomAttrs::All;


   // menubar = new QMenuBar(this);

    //创建定时器用于每帧更新
    timer = new QTimer();
    timer->setInterval(200);
    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    timer->start();

    this->btn_close.setStyleSheet(QStringLiteral(
        "QPushButton::hover {\n"
        "   background: red;\n"
        "   border : none;\n"
        "   color: white;\n"
        "}\n"));
    this->setStyleSheet(QStringLiteral(
        "QPushButton {\n"
        "   border : none;\n"
        "   border-radius: 3px;\n"
        "   min-height: 27px;\n"
        "   min-width:43px;\n"
        "   margin: 1px;\n"
        "   color: gray;\n"
        "}\n"
        "QPushButton::hover {\n"
        "   background: darkgrey;\n"
        "   border : none;\n"
        "   color: white;\n"
        "}\n"
        "QPushButton::pressed, QPushButton::!enabled {\n"
        "   background: grey;\n"
        "   border : none;\n"
        "   color: white;\n"
        "}\n"
        "titleBar { background: white; }\n"
    ));

    if (!parent) throw std::invalid_argument("Parent must be a QCustomWindow object (cannot be null).");
    this->m_parentWindow = parent;

//    QPixmap *pixmap = new QPixmap(":/EXE Icons/FRD.ico");
//    pixmap->scaled(this->FRD_icon.size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    this->FRD_icon.setScaledContents(true);
//    this->FRD_icon.setPixmap(*pixmap);

    this->lbl_windowTitle.setText("Fractal Designer");
    this->lbl_windowTitle.setAlignment(Qt::AlignCenter);

    this->btn_close.setIcon(QIcon(":/EXE Icons/Close.svg"));
    this->btn_maximize.setIcon(QIcon(":/EXE Icons/Maximize.svg"));
    this->btn_minimize.setIcon(QIcon(":/EXE Icons/Minimize.svg"));

//    this->FRD_icon.setMinimumSize(FRAME_BUTTON_SIZE/1.5);
//    this->FRD_icon.setMaximumSize(FRAME_BUTTON_SIZE/1.5);
    this->btn_close.setMaximumSize(FRAME_BUTTON_SIZE);
    this->btn_close.setMinimumSize(FRAME_BUTTON_SIZE);
    this->btn_maximize.setMaximumSize(FRAME_BUTTON_SIZE);
    this->btn_maximize.setMinimumSize(FRAME_BUTTON_SIZE);
    this->btn_minimize.setMaximumSize(FRAME_BUTTON_SIZE);
    this->btn_minimize.setMinimumSize(FRAME_BUTTON_SIZE);
    this->btn_minimize.setFlat(true);
    this->btn_maximize.setFlat(true);
    this->btn_close.setFlat(true);
    //addFRDmenu();

    this->m_layout.addWidget(&this->FRD_icon);
    //this->m_layout.addWidget(this->menubar);
    this->m_layout.addWidget(&this->lbl_windowTitle, 1);
    this->m_layout.addWidget(&this->btn_minimize);
    this->m_layout.addWidget(&this->btn_maximize);
    this->m_layout.addWidget(&this->btn_close);
    this->m_layout.setContentsMargins(0, 0, 0, 4);
    this->m_layout.setSpacing(0);

    this->setLayout(&this->m_layout);

    connect(&this->btn_close, &QPushButton::clicked, [this]{ emit this->requestClose(); });
    connect(&this->btn_minimize, &QPushButton::clicked, [this]{ emit this->requestMinimize(); });
    connect(&this->btn_maximize, &QPushButton::clicked, [this]{ emit this->requestMaximize(); });
   // connect(&this->btn_maximize, SIGNAL(clicked()), this, SLOT(updateMaxbutton()));
    connect(this, &QWidget::windowTitleChanged, &this->lbl_windowTitle, &QLabel::setText);

    this->setMaximumHeight(35);
    this->setMinimumHeight(35);

}

FRD_TitleBar::~FRD_TitleBar() {

    delete FileMenu ;
    delete EditMenu ;
    delete ViewMenu ;
    delete TemplateMenu;
    delete ImageMenu ;
    delete VideoMenu ;
    delete HelpMenu ;
    delete OptionsMenu;
    delete menubar;
}

void FRD_TitleBar::setWindowButtons(QCustomAttrs::WindowButtons btns){
    this->m_frameButtons = btns;
    this->btn_close.setVisible(btns & QCustomAttrs::Close);
    this->btn_maximize.setVisible(btns & QCustomAttrs::Maximize);
    this->btn_minimize.setVisible(btns & QCustomAttrs::Minimize);
}

void FRD_TitleBar::setWindowButtonEnabled(QCustomAttrs::WindowButton btn, bool enabled){
    if (btn & QCustomAttrs::Close) this->btn_close.setEnabled(enabled);
    if (btn & QCustomAttrs::Maximize) this->btn_maximize.setEnabled(enabled);
    if (btn & QCustomAttrs::Minimize) this->btn_minimize.setEnabled(enabled);
}

void FRD_TitleBar::addFRDmenu()
{
    FileMenu = new QMenu(tr("File"),this);
    EditMenu = new QMenu(tr("&Edit"),this);
    ViewMenu = new QMenu(tr("&View"),this);
    TemplateMenu = new QMenu(tr("&Template"),this);
    ImageMenu = new QMenu(tr("&Image"),this);
    VideoMenu = new QMenu(tr("&Viedo"),this);
    HelpMenu = new QMenu(tr("&Help"),this);
    OptionsMenu = new QMenu(tr("&Options"),this);

    FileMenu->addAction("New");
    FileMenu->addAction("Open");
    FileMenu->addAction("Save");
    FileMenu->addAction("Save As");
    FileMenu->addAction("Close");
    FileMenu->addAction("Rename");
    FileMenu->addAction("FRD 4 Compatibility");
    FileMenu->addAction("Exit");
    FileMenu->addAction("Delete");

    EditMenu->addAction("Undo");
    EditMenu->addAction("Redo");
    EditMenu->addAction("Delete All Parameters");

    ViewMenu->addAction("Preview");
    ViewMenu->addAction("Preview Refresh");
    ViewMenu->addAction("Magnifier");
    ViewMenu->addAction("Magnify Settings");

    TemplateMenu->addAction("Additional Template Settings");

    ImageMenu->addAction("Create Image");
    ImageMenu->addAction("Create Image in Range");
    ImageMenu->addAction("Stop");
    ImageMenu->addAction("Check Images");
    ImageMenu->addAction("Delete Images");
    ImageMenu->addAction("Thread Images");
    ImageMenu->addAction("Color Definition Auto Save");
    ImageMenu->addAction("Reset Color Definition");
    ImageMenu->addAction("High Quailty Mode");
    ImageMenu->addAction("Route Tool");
    ImageMenu->addAction("Format");
    ImageMenu->addAction("Alpha Channel");
    ImageMenu->addAction("Layer");
    ImageMenu->addAction("Picture Layer Settings");
    ImageMenu->addAction("Watermark Layer Settings");

    VideoMenu->addAction("Creat Video");
    VideoMenu->addAction("Format");
    VideoMenu->addAction("Install FFmpeg");

    HelpMenu->addAction("View Help");
    HelpMenu->addAction("Version");
    HelpMenu->addAction("Check Update");
    HelpMenu->addAction("Github Repository");
    HelpMenu->addAction("New Features");
    HelpMenu->addAction("Bug Report");
    HelpMenu->addAction("About Teddy van Jerry");
    HelpMenu->addAction("Sample Video");
    HelpMenu->addAction("Wizard");

    OptionsMenu->addMenu("Language");
    OptionsMenu->addMenu("Theme");

    menubar->addMenu(FileMenu);
    menubar->addMenu(EditMenu);
    menubar->addMenu(ViewMenu);
    menubar->addMenu(TemplateMenu);
    menubar->addMenu(ImageMenu);
    menubar->addMenu(VideoMenu);
    menubar->addMenu(HelpMenu);
    menubar->addMenu(OptionsMenu);

    // m_layout.addWidget(menubar);
}

void FRD_TitleBar::timeUpdate()
{
    // check whether the cursor is moving
    QPoint CurrentPos = mapFromGlobal(QCursor::pos());
    if (isinSide(rightBottomHit, CurrentPos)|| bResizeByRbHit)
        {
            if(cursor() != Qt::SizeFDiagCursor)
                this->setCursor(Qt::SizeFDiagCursor);
        }
//        else if (isinSide(rightTopHit, CurrentPos)|| bResizeByRtHit)
//        {
//            if(cursor() != Qt::SizeBDiagCursor)
//                this->setCursor(Qt::SizeBDiagCursor);
//        }
//        else if (isinSide(leftBottomHit, CurrentPos)|| bResizeByLbHit)
//        {
//            if(cursor() != Qt::SizeBDiagCursor)
//                this->setCursor(Qt::SizeBDiagCursor);
//        }
//        else if (isinSide(leftTopHit, CurrentPos)|| bResizeByLtHit)
//        {
//            if(cursor() != Qt::SizeFDiagCursor)
//                this->setCursor(Qt::SizeFDiagCursor);
//        }
//        else if (isinSide(topHit, CurrentPos)|| bResizeByTopHit)
//        {
//            if(cursor() != Qt::SizeVerCursor)
//                this->setCursor(Qt::SizeVerCursor);
//        }
//        else if (isinSide(bottomHit, CurrentPos) || bResizeByBottomHit)
//        {
//            if(cursor() != Qt::SizeVerCursor)
//                this->setCursor(Qt::SizeVerCursor);
//        }
//        else if (isinSide(leftHit, CurrentPos) || bResizeByLeftHit)
//        {
//            if(cursor() != Qt::SizeHorCursor)
//                this->setCursor(Qt::SizeHorCursor);
//        }
//        else if (isinSide(rightHit, CurrentPos)|| bResizeByRightHit)
//        {
//            if(cursor() != Qt::SizeHorCursor)
//                this->setCursor(Qt::SizeHorCursor);
//        }
        else
        {
            if(cursor() != Qt::ArrowCursor)
                this->setCursor(Qt::ArrowCursor);
    }
}

//void FRD_TitleBar::updateMaxbutton()
//{
//    if(this->m_parentWindow->isMaximized())
//    {
//        this->btn_maximize.setIcon(QIcon(":/EXE Icons/Restore.svg"));
//        qDebug() << "Restore icon";
//    }
//    else if(!this->m_parentWindow->isMaximized())
//    {
//        this->btn_maximize.setIcon(QIcon(":/EXE Icons/Maximize.svg"));
//        qDebug() << "Maximize icon";
//    }
//}


void FRD_TitleBar::updateTitleBar()
{
    //更新位置和大小
    topHit			= QRect(borderSize, 0, this->width() - borderSize * 2, borderSize);
    bottomHit		= QRect(borderSize, this->height() - borderSize, this->width() - borderSize * 2, borderSize);
    leftHit			= QRect(0, borderSize, borderSize, this->height() - borderSize * 2);
    rightHit		= QRect(this->width() - borderSize, borderSize, borderSize, this->height() - borderSize * 2);
    rightBottomHit	= QRect(this->width() - borderSize, this->height() - borderSize, borderSize, borderSize);
    rightTopHit		= QRect(this->width() - borderSize, 0, borderSize, borderSize);
    leftBottomHit	= QRect(0, this->height() - borderSize, borderSize, borderSize);
    leftTopHit		= QRect(0, 0, borderSize, borderSize);
}

void FRD_TitleBar::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    //opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    QWidget::paintEvent(event);
    updateTitleBar();
}

void FRD_TitleBar::mousePressEvent(QMouseEvent *event){
    if (event->button() & Qt::LeftButton){
        this->canMove = (event->x() > 5 && event->y() > 5 && event->x() < (this->m_parentWindow->width() - 5));
        this->m_pCursor = event->globalPos() - this->m_parentWindow->geometry().topLeft();
    }
    QWidget::mousePressEvent(event);
    updateTitleBar();
}

void FRD_TitleBar::mouseMoveEvent(QMouseEvent *event){
    if (!this->maximizing && canMove && event->buttons() & Qt::LeftButton
            && !this->m_parentWindow->isMaximized()) this->m_parentWindow->move(event->globalPos() - m_pCursor);
    this->maximizing = false;
    QWidget::mouseMoveEvent(event);
    updateTitleBar();
}

void FRD_TitleBar::mouseDoubleClickEvent(QMouseEvent *event){
    if (m_frameButtons & QCustomAttrs::Maximize && btn_maximize.isEnabled()
            && event->buttons() & Qt::LeftButton) {
        this->maximizing = true;
        emit requestMaximize();

    }
    QWidget::mouseDoubleClickEvent(event);
    updateTitleBar();
}

bool FRD_TitleBar::isinSide(QRect r, QPoint p)
{
    if (p.x() >= r.left() && p.x() <= r.right() && p.y() >= r.top() && p.y() <= r.bottom())
        return true;
    else
        return false;
}

