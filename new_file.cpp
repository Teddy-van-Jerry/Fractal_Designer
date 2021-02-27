#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QSettings>
#include <QMessageBox>
#include <QImage>
//#include <Windows.h>
//#include <shlwapi.h>
#include "new_file.h"
#include "ui_new_file.h"
#include "mainwindow.h"
//#pragma comment(lib,"shlwapi.lib")

New_File::New_File(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::New_File)
{
    ui->setupUi(this);
    MainWindow* p = (MainWindow*) parentWidget();
    if(p->Version_Higher_Than_4)
    {
        this->setWindowTitle("New Project");
    }
    else
    {
        this->setWindowTitle("New Project - Compatibility Mode");
    }
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

    if(p->Version_Higher_Than_4)
    {
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
    }
    else
    {
        target_dir.mkdir(new_path);

        if(target_dir.exists())
        {
        /*
            QString curDir = target_dir.path();
            QString icoPath = curDir + "/icon.png";
            QString iniPath = curDir + "/setinfo.ini";

            QImage Folder_Icon(":/exe_icon/EXE Icons/FRD_icon.png");
            Folder_Icon.save(icoPath);

            QFileInfo fileInfo(icoPath);
            if(!fileInfo.isFile())
            {
               QFile::copy(":/exe_icon/EXE Icons/FRD_icon.png", icoPath);
               // set icon.ico as a hidden file
               SetFileAttributes((LPCWSTR)icoPath.unicode(), FILE_ATTRIBUTE_HIDDEN);
               qDebug() << "icopath:" <<icoPath;
            }

            QFileInfo iniFileInfo(iniPath);
            if(!iniFileInfo.isFile())
            {
                QSettings settings(iniPath , QSettings::IniFormat); // the INI file in the current folder
                // set setinfo.ini to be SYSTEM and HIDDEN
                settings.beginGroup(".ShellClassInfo");
                settings.setValue("IconResource", icoPath);
                settings.endGroup();
                SetFileAttributes((LPCWSTR)iniPath.unicode(), FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);
                PathMakeSystemFolder((LPCWSTR)fileInfo.path().unicode());
            }
        */

            QMessageBox::information(this, tr("info"), "New Project Added:\n" + new_path);

            p->Project_Name = new_path;
            p->setWindowTitle("Fractal Designer - " + p->Project_Name);
        }
        else
        {
            QMessageBox::warning(this, tr("info"), tr("Failed to add the new project!"));
        }
    }

    close();
}

void New_File::on_new_browse_clicked()
{
    QString path_b = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("View Folder"), QDir::homePath()));
    ui->new_path_edit->setText(path_b);
}
