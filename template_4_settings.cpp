#include "template_4_settings.h"
#include "ui_template_4_settings.h"
#include "mainwindow.h"

#define info p->buff_info

Template_4_Settings::Template_4_Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Template_4_Settings)
{
    ui->setupUi(this);
    uiInit();
}

Template_4_Settings::~Template_4_Settings()
{
    delete ui;
}

void Template_4_Settings::uiInit()
{
    MainWindow* p = (MainWindow*) parentWidget();

    QList<QDoubleSpinBox*> spb_xn {
        ui->doubleSpinBox_x0_1,
        ui->doubleSpinBox_x1_1,
        ui->doubleSpinBox_x2_1,
        ui->doubleSpinBox_x3_1,
        ui->doubleSpinBox_x4_1,
        ui->doubleSpinBox_x5_1,
        ui->doubleSpinBox_x6_1,
        ui->doubleSpinBox_x7_1,
        ui->doubleSpinBox_x8_1,
        ui->doubleSpinBox_x9_1,
        ui->doubleSpinBox_x0_2,
        ui->doubleSpinBox_x1_2,
        ui->doubleSpinBox_x2_2,
        ui->doubleSpinBox_x3_2,
        ui->doubleSpinBox_x4_2,
        ui->doubleSpinBox_x5_2,
        ui->doubleSpinBox_x6_2,
        ui->doubleSpinBox_x7_2,
        ui->doubleSpinBox_x8_2,
        ui->doubleSpinBox_x9_2,
        ui->doubleSpinBox_x0_3,
        ui->doubleSpinBox_x1_3,
        ui->doubleSpinBox_x2_3,
        ui->doubleSpinBox_x3_3,
        ui->doubleSpinBox_x4_3,
        ui->doubleSpinBox_x5_3,
        ui->doubleSpinBox_x6_3,
        ui->doubleSpinBox_x7_3,
        ui->doubleSpinBox_x8_3,
        ui->doubleSpinBox_x9_3,
        ui->doubleSpinBox_x0_4,
        ui->doubleSpinBox_x1_4,
        ui->doubleSpinBox_x2_4,
        ui->doubleSpinBox_x3_4,
        ui->doubleSpinBox_x4_4,
        ui->doubleSpinBox_x5_4,
        ui->doubleSpinBox_x6_4,
        ui->doubleSpinBox_x7_4,
        ui->doubleSpinBox_x8_4,
        ui->doubleSpinBox_x9_4
    };

    ui->doubleSpinBox_a_1->setValue(info.Newton_a_1.getReal());
    ui->doubleSpinBox_a_2->setValue(info.Newton_a_1.getImaginary());
    ui->doubleSpinBox_a_3->setValue(info.Newton_a_2.getReal());
    ui->doubleSpinBox_a_4->setValue(info.Newton_a_2.getImaginary());

    for (int i = 0; i != 10; i++)
    {
        spb_xn[i     ]->setValue(info.Newton_xn_1[i].getReal());
        spb_xn[i + 10]->setValue(info.Newton_xn_1[i].getImaginary());
        spb_xn[i + 20]->setValue(info.Newton_xn_2[i].getReal());
        spb_xn[i + 30]->setValue(info.Newton_xn_2[i].getImaginary());
    }

    ui->doubleSpinBox_sin_1->setValue(info.Newton_sin_1.getReal());
    ui->doubleSpinBox_sin_2->setValue(info.Newton_sin_1.getImaginary());
    ui->doubleSpinBox_sin_3->setValue(info.Newton_sin_2.getReal());
    ui->doubleSpinBox_sin_4->setValue(info.Newton_sin_2.getImaginary());

    ui->doubleSpinBox_cos_1->setValue(info.Newton_cos_1.getReal());
    ui->doubleSpinBox_cos_2->setValue(info.Newton_cos_1.getImaginary());
    ui->doubleSpinBox_cos_3->setValue(info.Newton_cos_2.getReal());
    ui->doubleSpinBox_cos_4->setValue(info.Newton_cos_2.getImaginary());

    ui->doubleSpinBox_ex_1->setValue(info.Newton_ex_1.getReal());
    ui->doubleSpinBox_ex_2->setValue(info.Newton_ex_1.getImaginary());
    ui->doubleSpinBox_ex_3->setValue(info.Newton_ex_2.getReal());
    ui->doubleSpinBox_ex_4->setValue(info.Newton_ex_2.getImaginary());

    ui->slider->setValue(info.Newton_c_rate * 50);
}

void Template_4_Settings::on_pushButton_Cancel_clicked()
{
    close();
}

void Template_4_Settings::on_pushButton_Save_clicked()
{
    MainWindow* p = (MainWindow*) parentWidget();

    QList<double> spb_xn {
        ui->doubleSpinBox_x0_1->value(),
        ui->doubleSpinBox_x1_1->value(),
        ui->doubleSpinBox_x2_1->value(),
        ui->doubleSpinBox_x3_1->value(),
        ui->doubleSpinBox_x4_1->value(),
        ui->doubleSpinBox_x5_1->value(),
        ui->doubleSpinBox_x6_1->value(),
        ui->doubleSpinBox_x7_1->value(),
        ui->doubleSpinBox_x8_1->value(),
        ui->doubleSpinBox_x9_1->value(),
        ui->doubleSpinBox_x0_2->value(),
        ui->doubleSpinBox_x1_2->value(),
        ui->doubleSpinBox_x2_2->value(),
        ui->doubleSpinBox_x3_2->value(),
        ui->doubleSpinBox_x4_2->value(),
        ui->doubleSpinBox_x5_2->value(),
        ui->doubleSpinBox_x6_2->value(),
        ui->doubleSpinBox_x7_2->value(),
        ui->doubleSpinBox_x8_2->value(),
        ui->doubleSpinBox_x9_2->value(),
        ui->doubleSpinBox_x0_3->value(),
        ui->doubleSpinBox_x1_3->value(),
        ui->doubleSpinBox_x2_3->value(),
        ui->doubleSpinBox_x3_3->value(),
        ui->doubleSpinBox_x4_3->value(),
        ui->doubleSpinBox_x5_3->value(),
        ui->doubleSpinBox_x6_3->value(),
        ui->doubleSpinBox_x7_3->value(),
        ui->doubleSpinBox_x8_3->value(),
        ui->doubleSpinBox_x9_3->value(),
        ui->doubleSpinBox_x0_4->value(),
        ui->doubleSpinBox_x1_4->value(),
        ui->doubleSpinBox_x2_4->value(),
        ui->doubleSpinBox_x3_4->value(),
        ui->doubleSpinBox_x4_4->value(),
        ui->doubleSpinBox_x5_4->value(),
        ui->doubleSpinBox_x6_4->value(),
        ui->doubleSpinBox_x7_4->value(),
        ui->doubleSpinBox_x8_4->value(),
        ui->doubleSpinBox_x9_4->value()
    };

    bool edited = false;

    if (info.Newton_a_1.getReal()      != ui->doubleSpinBox_a_1->value()) { info.Newton_a_1.setReal     (ui->doubleSpinBox_a_1->value()); edited = true; }
    if (info.Newton_a_1.getImaginary() != ui->doubleSpinBox_a_2->value()) { info.Newton_a_1.setImaginary(ui->doubleSpinBox_a_2->value()); edited = true; }
    if (info.Newton_a_2.getReal()      != ui->doubleSpinBox_a_3->value()) { info.Newton_a_2.setReal     (ui->doubleSpinBox_a_3->value()); edited = true; }
    if (info.Newton_a_2.getImaginary() != ui->doubleSpinBox_a_4->value()) { info.Newton_a_2.setImaginary(ui->doubleSpinBox_a_4->value()); edited = true; }

    for (int i = 0; i != 10; i++)
    {
        if (info.Newton_xn_1[i].getReal()      != spb_xn[i     ]) { info.Newton_xn_1[i].setReal     (spb_xn[i     ]); edited = true; }
        if (info.Newton_xn_1[i].getImaginary() != spb_xn[i + 10]) { info.Newton_xn_1[i].setImaginary(spb_xn[i + 10]); edited = true; }
        if (info.Newton_xn_2[i].getReal()      != spb_xn[i + 20]) { info.Newton_xn_2[i].setReal     (spb_xn[i + 20]); edited = true; }
        if (info.Newton_xn_2[i].getImaginary() != spb_xn[i + 30]) { info.Newton_xn_2[i].setImaginary(spb_xn[i + 30]); edited = true; }
    }

    if (info.Newton_sin_1.getReal()      != ui->doubleSpinBox_sin_1->value()) { info.Newton_sin_1.setReal     (ui->doubleSpinBox_sin_1->value()); edited = true; }
    if (info.Newton_sin_1.getImaginary() != ui->doubleSpinBox_sin_2->value()) { info.Newton_sin_1.setImaginary(ui->doubleSpinBox_sin_2->value()); edited = true; }
    if (info.Newton_sin_2.getReal()      != ui->doubleSpinBox_sin_3->value()) { info.Newton_sin_2.setReal     (ui->doubleSpinBox_sin_3->value()); edited = true; }
    if (info.Newton_sin_2.getImaginary() != ui->doubleSpinBox_sin_4->value()) { info.Newton_sin_2.setImaginary(ui->doubleSpinBox_sin_4->value()); edited = true; }

    if (info.Newton_cos_1.getReal()      != ui->doubleSpinBox_cos_1->value()) { info.Newton_cos_1.setReal     (ui->doubleSpinBox_cos_1->value()); edited = true; }
    if (info.Newton_cos_1.getImaginary() != ui->doubleSpinBox_cos_2->value()) { info.Newton_cos_1.setImaginary(ui->doubleSpinBox_cos_2->value()); edited = true; }
    if (info.Newton_cos_2.getReal()      != ui->doubleSpinBox_cos_3->value()) { info.Newton_cos_2.setReal     (ui->doubleSpinBox_cos_3->value()); edited = true; }
    if (info.Newton_cos_2.getImaginary() != ui->doubleSpinBox_cos_4->value()) { info.Newton_cos_2.setImaginary(ui->doubleSpinBox_cos_4->value()); edited = true; }

    if (info.Newton_ex_1.getReal()      != ui->doubleSpinBox_ex_1->value()) { info.Newton_ex_1.setReal     (ui->doubleSpinBox_ex_1->value()); edited = true; }
    if (info.Newton_ex_1.getImaginary() != ui->doubleSpinBox_ex_2->value()) { info.Newton_ex_1.setImaginary(ui->doubleSpinBox_ex_2->value()); edited = true; }
    if (info.Newton_ex_2.getReal()      != ui->doubleSpinBox_ex_3->value()) { info.Newton_ex_2.setReal     (ui->doubleSpinBox_ex_3->value()); edited = true; }
    if (info.Newton_ex_2.getImaginary() != ui->doubleSpinBox_ex_4->value()) { info.Newton_ex_2.setImaginary(ui->doubleSpinBox_ex_4->value()); edited = true; }

    if (info.Newton_c_rate != static_cast<double>(ui->slider->value()) / 50)
    {
        info.Newton_c_rate = static_cast<double>(ui->slider->value()) / 50;
        edited = true;
    }

    if (edited) p->edit(EDIT_ALREADY);

    close();
}

void Template_4_Settings::on_pushButton_Revert_clicked()
{
    uiInit();
}

void Template_4_Settings::on_pushButton_Help_clicked()
{
    QMessageBox::information(this, "Help", "Newton Fractal");
}
