#include "template_2_settings.h"
#include "ui_template_2_settings.h"

Template_2_Settings::Template_2_Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Template_2_Settings)
{
    ui->setupUi(this);
}

Template_2_Settings::~Template_2_Settings()
{
    delete ui;
}
