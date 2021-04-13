#include "help.h"
#include "ui_help.h"
#include "mainwindow.h"

Help::Help(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);
}

Help::~Help()
{
    delete ui;
}

void Help::on_actionBack_to_main_window_triggered()
{
    close();
}

void Help::on_actionNew_Features_triggered()
{
    MainWindow* p = (MainWindow*)parentWidget();
    p->NewFeatures();
}

void Help::on_actionChinese_Version_triggered()
{
    QDesktopServices::openUrl(QUrl(Chinese_Help_Url));
}
