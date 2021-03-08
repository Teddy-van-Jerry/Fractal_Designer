#include "create_image_info.h"
#include "ui_create_image_info.h"

Create_Image_Info::Create_Image_Info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Create_Image_Info),
    index(0)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    timer->start(50);
    if(!time_left.isValid())
        qDebug() << "invalid";
}

Create_Image_Info::~Create_Image_Info()
{
    delete ui;
    delete timer;
}

void Create_Image_Info::set_info(QString Name, QString Format, int Total, int Start)
{
    qDebug() << "Set Info Successfully" << Total << Start;
    name = Name;
    format = Format;
    total = Total;
    index = Start;
    start = Start;
    usingQList = false;
}

void Create_Image_Info::set_info_(QString Name, QString Format, QList<int> list)
{
    // qDebug() << "Set Info Successfully" << Total << Start;
    name = Name;
    format = Format;
    total = list.size();
    start = 0;
    index = 0;
    list_ = list;
    usingQList = true;
}

void Create_Image_Info::updateTime()
{
    // qDebug() << "updateTime";
    time_now = time_now.addMSecs(50);
    time_one = time_one.addMSecs(50);
    if(time_left.hour() + time_left.minute() + time_left.second() > 0)
    {
        time_left = time_left.addMSecs(-50);
    }
    if(usingQList)
    {
        ui->label_now->setText(tr("Now: ") + name + QString::number(list_[index]) + "." + format);
    }
    else
    {
        ui->label_now->setText(tr("Now: ") + name + QString::number(index) + "." + format);
    }
    ui->label_progress->setText(tr("Progress: ") + QString::number(index - start) + " / " + QString::number(total));
    ui->label_timeUsed->setText(tr("Time Used: ") + time_now.toString("hh:mm:ss"));
    ui->label_leftTime->setText(tr("Estimated left time: ") + time_left.toString("hh:mm:ss"));
}

void Create_Image_Info::updateInfo()
{
    qDebug() << "updateInfo";
    index++;
    int size_of_image = 1024;
    if(usingQList)
    {
        QFile already_created_image(name + QString::number(list_[index]) + "." + format);
        if(already_created_image.exists())
        {
            size_of_image = already_created_image.size();
        }
    }
    else
    {
        QFile already_created_image(name + QString::number(index) + "." + format);
        if(already_created_image.exists())
        {
            size_of_image = already_created_image.size();
        }
    }
    double time_for_one = 3600 * time_one.hour() + 60 * time_one.minute() + time_one.second() + time_one.msec() / 1000.0;
    double speed = (time_for_one < 0.05)
            ? (11 * size_of_image / 1024.0)
            : (size_of_image / 1024.0 / time_for_one);
    double estimated_left_time = time_for_one * (total + start - index);
    qDebug() << estimated_left_time;
    time_left.setHMS(0, 0, 0, 0);
    time_left = time_left.addMSecs(1000 * estimated_left_time);
    //time_left.setHMS((int)estimated_left_time / 3600, (int)estimated_left_time / 60 % 60, (int) estimated_left_time % 60);
    if(usingQList)
    {
        ui->label_now->setText(tr("Now: ") + name + QString::number(list_[index]) + "." + format);
    }
    else
    {
        ui->label_now->setText(tr("Now: ") + name + QString::number(index) + "." + format);
    }
    ui->label_speed->setText(tr("Speed: ") + QString::number(speed, 'g', 4) + " kb/s");
    ui->label_progress->setText(tr("Progress: ") + QString::number(index - start) + " / " + QString::number(total));
    ui->label_timeUsed->setText(tr("Time Used: ") + time_now.toString("hh:mm:ss"));
    ui->label_leftTime->setText(tr("Estimated left time: ") + time_left.toString("hh:mm:ss"));
    ui->progressBar->setValue(100 * (index - start) / total);
    time_one.setHMS(0, 0, 0, 0);
    if(index - start == total)
    {
        timer->stop();
        ui->label_Creating->setText("Finished!");
        ui->label_leftTime->setVisible(false);
        ui->label_speed->setVisible(false);
        ui->label_now->setVisible(false);
    }
}

void Create_Image_Info::close_create_image_info()
{

}

void Create_Image_Info::closeEvent(QCloseEvent* event)
{
    timer->stop();
}
