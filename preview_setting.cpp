#include "preview_setting.h"
#include "ui_preview_setting.h"
#include "mainwindow.h"

Preview_Setting::Preview_Setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Preview_Setting)
{
    ui->setupUi(this);
}

#define pa static_cast<MainWindow*>(parent())->buff_info.ps

Preview_Setting::~Preview_Setting()
{
    delete ui;
}

void Preview_Setting::closeEvent(QCloseEvent*)
{
    if (edited)
    {
        static_cast<MainWindow*>(parent())->edit(EDIT_ALREADY);
        // reset the status
        edited = false;
    }
}

void Preview_Setting::on_toolButton_clicked()
{
    QMessageBox::information(this, tr("Help"),
        tr("<h2>Preview Settings</h2>"
           "<h3>Preview Image Size</h3>"
           "<p>This is the actual size of the image created.</p>"
           "<p>The size is measured by pixel.</p>"
           "<p>By default, it is 800 &times; 600.</p>"
           "<h3>Preview Scale</h3>"
           "<p>The size here is the mathematical scale in fractal.</p>"
           "<h3>Preview Location</h3>"
           "<p>It specifies the centre coordinate of preview image.</p>"
           "<h3>Other infomation</h3>"
           "<P>If you are not sure about the value, you can use <i>Default</i> button.</P>"
           "<p>Burning Ship Fractal has two default options.</p>"));
}

void Preview_Setting::on_spinBox_Width_valueChanged(int arg1)
{
    pa.width = arg1;
    edited = true;
}

void Preview_Setting::on_spinBox_Height_valueChanged(int arg1)
{
    pa.height = arg1;
    edited = true;
}
void Preview_Setting::on_doubleSpinBox_xWidth_valueChanged(double arg1)
{
    pa.xWidth = arg1;
    edited = true;
}

void Preview_Setting::on_doubleSpinBox_yHeight_valueChanged(double arg1)
{
    pa.yHeight = arg1;
    edited = true;
}

void Preview_Setting::on_doubleSpinBox_angle_valueChanged(double arg1)
{
    pa.angle = arg1;
    edited = true;
}

void Preview_Setting::on_doubleSpinBox_X_valueChanged(double arg1)
{
    pa.centreX = arg1;
    edited = true;
}

void Preview_Setting::on_doubleSpinBox_Y_valueChanged(double arg1)
{
    pa.centreY = arg1;
    edited = true;
}

void Preview_Setting::on_checkBox_stateChanged(int arg1)
{
    pa.autoRefresh = arg1;
    edited = true;
}

void Preview_Setting::on_pushButton_Default_clicked()
{
    MainWindow* p = (MainWindow*)parent();
    switch(p->buff_info.template_)
    {
    case 1: // Mandelbrot Set
        setTemplateInfo(800, 600, 3.2, 2.4, 0, -0.7, 0, ui->checkBox->isChecked());
        break;
    case 2: // Julia Set
        setTemplateInfo(800, 600, 3.2, 2.4, 0, 0, 0, ui->checkBox->isChecked());
        break;
    case 3: // Burning Ship Fractal
    {
        QMessageBox msg_box;
        msg_box.setWindowTitle(tr("Burning Ship Fractal"));
        msg_box.setText("There are mainly two views of Burning Ship Fractal,\nplease choose which one do you prefer.");
        QPushButton* btn1 = msg_box.addButton(tr("Universal View"), QMessageBox::ActionRole);
        QPushButton* btn2 = msg_box.addButton(tr("Burning Ship View"), QMessageBox::ActionRole);
        msg_box.addButton(QMessageBox::Cancel);
        msg_box.setDefaultButton(btn1);
        auto choice = msg_box.exec();
        if (choice == QMessageBox::Cancel) return;
        else if (msg_box.clickedButton() == btn1)
        {
            // Universal View
            setTemplateInfo(800, 600, 4, 3, 0, -0.4, 0.6, ui->checkBox->isChecked());
        }
        else if (msg_box.clickedButton() == btn2)
        {
            // Burning Ship View
            setTemplateInfo(800, 600, 0.16, 0.12, 0, -1.755, 0.032, ui->checkBox->isChecked());
        }
    }
        break;
    case 4:
        setTemplateInfo(800, 600, 3.2, 2.4, 0, 0, 0, ui->checkBox->isChecked());
        break;
    case 5:
        setTemplateInfo(800, 600, 4, 3, 0, 0, 0, ui->checkBox->isChecked());
        break;
    default:
        break;
    }
}

//
// preview->setImage(0, 0, 3.2, 2.4, 800, 600, 0, ui->doubleSpinBox_t->value(), "png", Pre_Img_Dir, "Preview Image", "Preview");

/*
if(curr_info.template_ == 1)
    preview->setImage(-0.7, 0, 3.2, 2.4, 800, 600, 0, ui->doubleSpinBox_t->value(), "png", Pre_Img_Dir, "Preview Image", "Preview");
if(curr_info.template_ == 3)
    preview->setImage(-0.4, 0.6, 4, 3, 800, 600, 0, ui->doubleSpinBox_t->value(), "png", Pre_Img_Dir, "Preview Image", "Preview");
if(curr_info.template_ == 5)
    preview->setImage(0, 0, 4, 3, 800, 600, 0, ui->doubleSpinBox_t->value(), "png", Pre_Img_Dir, "Preview Image", "Preview");
*/

void Preview_Setting::setTemplateInfo(int width_, int height_, double xWidth_, double yHeight_, double angle_, double centreX_, double centreY_, bool checked)
{
    ui->spinBox_Width->setValue(width_);
    ui->spinBox_Height->setValue(height_);
    ui->doubleSpinBox_xWidth->setValue(xWidth_);
    ui->doubleSpinBox_yHeight->setValue(yHeight_);
    ui->doubleSpinBox_angle->setValue(angle_);
    ui->doubleSpinBox_X->setValue(centreX_);
    ui->doubleSpinBox_Y->setValue(centreY_);
    ui->checkBox->setChecked(checked);
}

void Preview_Setting::updateInfo()
{
    setTemplateInfo(pa.width, pa.height, pa.xWidth, pa.yHeight, pa.angle, pa.centreX, pa.centreY, pa.autoRefresh);
}

#undef pa

void Preview_Setting::on_pushButton_OK_clicked()
{
    close();
}

void Preview_Setting::on_pushButton_clicked()
{
    // No edit is made,
    // just close directly
    if (edited)
    {
        // Otherwise, revert to the original status

        MainWindow* p = (MainWindow*)parent();
        edited = false;

        if (!p->NO_EDIT)
        {
            p->NO_EDIT = true;
            // revert
            p->buff_info = p->pre_info[p->current_info_v];
            p->NO_EDIT = false;
        }
        else
        {
            // revert
            p->buff_info = p->pre_info[p->current_info_v];
        }
    }
    close();
}
