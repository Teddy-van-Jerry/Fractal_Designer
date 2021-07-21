#include "version.h"
#include "ui_version.h"
#include "mainwindow.h"

Version::Version(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Version)
{
    ui->setupUi(this);
}

Version::~Version()
{
    delete ui;
}

void Version::on_pushButton_Help_clicked()
{
    MainWindow* p = (MainWindow*)parentWidget();
    p->Version_Dialog_Support(1);
}

void Version::on_pushButton_New_clicked()
{
    MainWindow* p = (MainWindow*)parentWidget();
    p->Version_Dialog_Support(2);
}

void Version::on_pushButton_Update_clicked()
{
    MainWindow* p = (MainWindow*)parentWidget();
    p->Version_Dialog_Support(3);
}
