#include "new_features.h"
#include "ui_new_features.h"

New_Features::New_Features(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::New_Features)
{
    ui->setupUi(this);
    ui->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);

}

New_Features::~New_Features()
{
    delete ui;
}
