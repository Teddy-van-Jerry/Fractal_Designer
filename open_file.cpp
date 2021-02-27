#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QVector>
#include "open_file.h"
#include "ui_open_file.h"
#include "mainwindow.h"

Open_File::Open_File(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Open_File)
{
    ui->setupUi(this);
    ui->button_confirm_open->setDisabled(true);
    // this->setWindowTitle("Open Project");
}

Open_File::~Open_File()
{
    delete ui;
}

void Open_File::on_button_cancel_clicked()
{
    close();
}

void Open_File::on_button_confirm_open_clicked()
{
    MainWindow* p = (MainWindow*) parentWidget();
    if(p->Version_Higher_Than_4)
    {
        qDebug() << "Version error: Open with the low version";
    }
    else
    {
        p->Project_Name = ui->open_path_edit->text();
        QDir check_path = p->Project_Name;
        if(!check_path.exists())
        {
            QMessageBox::warning(this, "Warning", "The folder doesn't exist.");
            ui->open_path_edit->clear();
            ui->open_path_edit->setFocus();
            ui->button_confirm_open->setDisabled(true);
            return;
        }
        else if(p->Project_Name.right(4) != ".frd")
        {
            QMessageBox::warning(this, "Warning", "This is not the Fractal Designer project folder or its name has been changed.");
            ui->open_path_edit->clear();
            ui->open_path_edit->setFocus();
            ui->button_confirm_open->setDisabled(true);
            return;
        }
        else
        {
            ui->button_confirm_open->setDisabled(false);
            ui->open_path_edit->setText(p->Project_Name);
        }
        p->setWindowTitle("Fractal Designer - " + p->Project_Name);

        QFile pre_template(p->Project_Name + "/Template_Choice.txt");
        if(pre_template.exists())
        {
            int choice_pre = 0;
            pre_template.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&pre_template);
            in >> choice_pre;
            pre_template.close();
            emit template_choice(choice_pre);
            qDebug() << "emit template_choice:" << choice_pre;
        }

        QFile pre_value(p->Project_Name + "/Define_Value.txt");
        if(pre_value.exists())
        {
            double n1, n2;
            int n3;
            pre_value.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&pre_value);
            in >> n1 >> n2 >> n3;
            pre_value.close();
            emit define_value(n1, n2, n3);
            qDebug() << "emit define_value:" << n1 << " " << n2 << " " << n3;
        }

        QFile pre_size(p->Project_Name + "/Image_Size.txt");
        if(pre_size.exists())
        {
            int n1, n2;
            pre_size.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&pre_size);
            in >> n1 >> n2;
            pre_size.close();
            emit image_size(n1, n2);
            qDebug() << "emit image_size:" << n1 << " " << n2;
        }

        QFile pre_route(p->Project_Name + "/Route_Info.txt");
        if(pre_route.exists())
        {
            QVector<double> info;
            double one_info;
            int Row;
            pre_route.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&pre_route);
            in >> Row;
            for(int i = 0; i != 6 * Row; i++)
            {
                in >> one_info;
                info.push_back(one_info);
            }
            pre_route.close();
            emit route_info(info);
            qDebug() << "emit route_info (size):" << info.size();
        }

        QFile pre_image_path(p->Project_Name + "/Image_Dir.txt");
        if(pre_image_path.exists())
        {
            QString path = QDir::currentPath();
            pre_route.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&pre_image_path);
            in >> path;
            pre_image_path.close();
            emit image_path(path);
            qDebug() << "emit image_path:" << path;
        }

        QFile pre_image_prefix(p->Project_Name + "/Image_Prefix.txt");
        if(pre_image_prefix.exists())
        {
            QString prefix = "";
            pre_image_prefix.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&pre_image_prefix);
            in >> prefix;
            pre_image_prefix.close();
            emit image_prefix(prefix);
            qDebug() << "emit image_prefix:" << prefix;
        }

        QFile pre_fps(p->Project_Name + "/Frame_Rate.txt");
        if(pre_fps.exists())
        {
            int fps = 24;
            pre_fps.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&pre_fps);
            in >> fps;
            pre_fps.close();
            emit frame_rate(fps);
            qDebug() << "emit Frame Rate:" << fps;
        }

        QFile pre_total_time(p->Project_Name + "/Total_Time.txt");
        if(pre_total_time.exists())
        {
            QString time = "01:00";
            pre_total_time.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&pre_total_time);
            in >> time;
            pre_total_time.close();
            emit total_time(time);
            qDebug() << "emit total_time:" << time;
        }

        QFile pre_music_bro(p->Project_Name + "/Music_Added.txt");
        if(pre_music_bro.exists())
        {
            QVector<QString> contents;
            QString str;
            QTextStream in(&pre_music_bro);
            pre_music_bro.open(QIODevice::ReadOnly | QIODevice::Text);
            do
            {
                str = in.readLine();
                if(str != "") contents.push_back(str);
            }
            while (str != "<End>");
            contents.pop_back(); // pop back the <End>
            pre_music_bro.close();
            emit music_added(contents);
            qDebug() << "emit music_added";
        }
    }

    close();
}

void Open_File::on_open_path_edit_textEdited(const QString &arg1)
{
    if(arg1 != "")
    {
        ui->button_confirm_open->setDisabled(false);
    }
}

void Open_File::on_open_browse_clicked()
{
    QString Pro_Path = QDir::toNativeSeparators(QFileDialog::getExistingDirectory(this, tr("Open Project"), QDir::currentPath()));
    ui->button_confirm_open->setDisabled(false);
    ui->open_path_edit->setText(Pro_Path);
}
