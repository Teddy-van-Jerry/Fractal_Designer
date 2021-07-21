#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>
#include <QImage>
#include "new_file.h"
#include "ui_new_file.h"
#include "mainwindow.h"

New_File::New_File(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::New_File)
{
    ui->setupUi(this);
    MainWindow* p = (MainWindow*) parentWidget();

}

New_File::~New_File()
{
    delete ui;
}

void New_File::on_button_cancel_clicked()
{
    close();
}

void New_File::on_button_confirm_new_clicked()
{
    MainWindow* p = (MainWindow*) parentWidget();
    QDir target_dir;
    QString new_path;
    QString path = ui->new_path_edit->text();

    if(!QDir(path).exists() || path == "")
    {
        QMessageBox::critical(this, "Fail to create a new project", "The path you choose does not exist!");
        return;
    }

    if(path.right(1) != "\\")
    {
        path.append(tr("\\"));
    }

    if(!ui->new_name_edit->text().isEmpty())
    {
        new_path = path.append(ui->new_name_edit->text());
    }
    else
    {
        new_path = path.append(tr("untitled"));
    }

    if(new_path.right(4) != ".frd")
    {
        new_path.append(tr(".frd"));
    }

    // deal with overwrite late
    uint8_t md5[16] = {0};
    char magic[8] = {'F', 'R', 'D', -17, 'T', 'v', 'J', 42 }; // magic number : "FRD\xEFTvJ*"
    QFile FRD_N(new_path);
    FRD_N.open(QIODevice::WriteOnly);
    QDataStream out(&FRD_N);
    for(int i = 0; i != 16; i++) // 16
    {
        out << md5[i];
    }
    for(int i = 0; i != 8; i++) // 24
    {
        out << magic[i];
    }
    for(int i = 0; i != 4; i++) // 28
    {
        out << p->FRD_Version[i];
    }
    FRD_N.close();

    p->Open_Location = new_path;
    p->setWindowTitle("Fractal Designer - " + p->Open_Location);

    close();
}

void New_File::on_new_browse_clicked()
{
    QString path_b = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("View Folder"), QDir::homePath()));
    if (!path_b.isEmpty()) ui->new_path_edit->setText(path_b);
}
