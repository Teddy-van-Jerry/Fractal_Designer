#include "template_2_settings.h"
#include "ui_template_2_settings.h"
#include "mainwindow.h"
#define set_p MainWindow* p = (MainWindow*) parentWidget()
#define p_info p->buff_info

Template_2_Settings::Template_2_Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Template_2_Settings)
{
    ui->setupUi(this);
    set_p;
    this_NO_EDIT = true;
    ui->doubleSpinBox_C1R->setValue(p_info.Julia_c1.getReal());
    ui->doubleSpinBox_C1I->setValue(p_info.Julia_c1.getImaginary());
    ui->doubleSpinBox_C2R->setValue(p_info.Julia_c2.getReal());
    ui->doubleSpinBox_C2I->setValue(p_info.Julia_c2.getImaginary());
    ui->doubleSpinBox_Rate->setValue(p_info.Julia_c_rate);
    this_NO_EDIT = false;
}

Template_2_Settings::~Template_2_Settings()
{
    delete ui;
}

void Template_2_Settings::on_doubleSpinBox_C1R_valueChanged(double arg1)
{
    set_p;
    if(p->NO_EDIT || this_NO_EDIT)  return;
    p_info.Julia_c1.setReal(arg1);
    edited = true;
}

void Template_2_Settings::on_doubleSpinBox_C1I_valueChanged(double arg1)
{
    set_p;
    if(p->NO_EDIT || this_NO_EDIT) return;
    p_info.Julia_c1.setImaginary(arg1);
    edited = true;
}

void Template_2_Settings::on_doubleSpinBox_C2R_valueChanged(double arg1)
{
    set_p;
    if(p->NO_EDIT || this_NO_EDIT) return;
    p_info.Julia_c2.setReal(arg1);
    edited = true;
}

void Template_2_Settings::on_doubleSpinBox_C2I_valueChanged(double arg1)
{
    set_p;
    if(p->NO_EDIT || this_NO_EDIT) return;
    p_info.Julia_c2.setImaginary(arg1);
    edited = true;
}

void Template_2_Settings::on_doubleSpinBox_Rate_valueChanged(double arg1)
{
    set_p;
    if(p->NO_EDIT || this_NO_EDIT) return;
    p_info.Julia_c_rate = arg1;
    edited = true;
}

void Template_2_Settings::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);

    set_p;
    if(edited) p->edit(EDIT_ALREADY);
}
