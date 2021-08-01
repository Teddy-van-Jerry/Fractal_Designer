#ifndef PREVIEW_SETTING_H
#define PREVIEW_SETTING_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Preview_Setting;
}

class Preview_Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Preview_Setting(QWidget *parent = nullptr);
    ~Preview_Setting();

private slots:

    void closeEvent(QCloseEvent*);

    void on_toolButton_clicked();

    void on_spinBox_Width_valueChanged(int arg1);

    void on_spinBox_Height_valueChanged(int arg1);

    void on_doubleSpinBox_xWidth_valueChanged(double arg1);

    void on_doubleSpinBox_yHeight_valueChanged(double arg1);

    void on_doubleSpinBox_angle_valueChanged(double arg1);

    void on_doubleSpinBox_X_valueChanged(double arg1);

    void on_doubleSpinBox_Y_valueChanged(double arg1);

    void on_checkBox_stateChanged(int arg1);

    void on_pushButton_Default_clicked();

    void on_pushButton_OK_clicked();

    void on_pushButton_clicked();

public:

    void setTemplateInfo(int width_, int height_, double xWidth_, double yHeight_, double angle_, double centreX_, double centreY_, bool checked);

    void updateInfo();

private:
    Ui::Preview_Setting *ui;

    bool edited = false;
};

#endif // PREVIEW_SETTING_H
