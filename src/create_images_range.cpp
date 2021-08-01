#include "create_images_range.h"
#include "ui_create_images_range.h"
#include "mainwindow.h"

Create_Images_Range::Create_Images_Range(QWidget *parent, int limit_number) :
    QDialog(parent),
    ui(new Ui::Create_Images_Range),
    limit(limit_number)
{
    ui->setupUi(this);
    ui->spinBox_from->setMinimum(0);
    ui->spinBox_from->setMaximum(limit - 1);
    ui->spinBox_from->setValue(0);
    ui->spinBox_to->setMinimum(0);
    ui->spinBox_to->setMaximum(limit - 1);
    ui->spinBox_to->setValue(limit - 1);
}

Create_Images_Range::~Create_Images_Range()
{
    delete ui;
}

void Create_Images_Range::on_pushButton_go_clicked()
{
    if(ui->spinBox_from->value() > ui->spinBox_to->value())
    {
        QMessageBox::warning(this, "Error", "Illegal Range!");
        return;
    }
    MainWindow* p = (MainWindow*)parentWidget();
    p->from_i = ui->spinBox_from->value();
    p->to_i   = ui->spinBox_to->value();
    close();
}
