#ifndef TEMPLATE_2_SETTINGS_H
#define TEMPLATE_2_SETTINGS_H

#include <QDialog>

namespace Ui {
class Template_2_Settings;
}

class Template_2_Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Template_2_Settings(QWidget *parent = nullptr);
    ~Template_2_Settings();

private slots:
    void on_doubleSpinBox_C1R_valueChanged(double arg1);

    void on_doubleSpinBox_C1I_valueChanged(double arg1);

    void on_doubleSpinBox_C2R_valueChanged(double arg1);

    void on_doubleSpinBox_C2I_valueChanged(double arg1);

    void on_doubleSpinBox_Rate_valueChanged(double arg1);

    void closeEvent(QCloseEvent* event);

private:
    Ui::Template_2_Settings *ui;

    bool edited = false;

    bool this_NO_EDIT = false;
};

#endif // TEMPLATE_2_SETTINGS_H
