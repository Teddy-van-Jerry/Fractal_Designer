#ifndef TEMPLATE_4_SETTINGS_H
#define TEMPLATE_4_SETTINGS_H

#include <QDialog>

namespace Ui {
class Template_4_Settings;
}

class Template_4_Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Template_4_Settings(QWidget *parent = nullptr);
    ~Template_4_Settings();

private slots:
    void on_pushButton_Cancel_clicked();

    void on_pushButton_Save_clicked();

    void on_pushButton_Revert_clicked();

    void on_pushButton_Help_clicked();

private:

    void uiInit();

    Ui::Template_4_Settings *ui;
};

#endif // TEMPLATE_4_SETTINGS_H
