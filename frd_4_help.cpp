#include "frd_4_help.h"
#include "ui_frd_4_help.h"

FRD_4_Help::FRD_4_Help(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FRD_4_Help)
{
    ui->setupUi(this);
}

FRD_4_Help::~FRD_4_Help()
{
    delete ui;
}

void FRD_4_Help::on_actionBack_to_main_window_triggered()
{
    close();
}
