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

private:
    Ui::Template_2_Settings *ui;
};

#endif // TEMPLATE_2_SETTINGS_H
