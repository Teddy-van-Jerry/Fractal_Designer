#include "route_tool.h"
#include "ui_route_tool.h"
#include "mainwindow.h"

inline std::complex<double> _curr_complex(const std::complex<double>& c1, const std::complex<double>& c2, double t, double k)
{
    return c1 + (c2 - c1) * ((1 - k) * t + k * t * t);
}

class Paint_Event : public QDialog
{
public:
    explicit Paint_Event(QWidget* parent, int dx, int dy, const QImage& image)
        : p(parent), Dx(dx), Dy(dy), img(image)
    {
        // update();
    }
    ~Paint_Event() {}
    void paintEvent(QPaintEvent* event)
    {
        Q_UNUSED(event);
        QPainter painter(p);
        qDebug() << "paintEvent";
        painter.setRenderHint(QPainter::Antialiasing, true);
        QPixmap view_image = QPixmap::fromImage(img);
        QTransform translation;
        //translation.scale(0.5, 0.5);
        translation.translate(Dx, Dy);
        painter.setTransform(translation);
        //view_image.scroll(Dx, Dy, 0, 0, 600, 600);
        QPixmap view_image_ = view_image.transformed(translation);
        //ui->Route_View->setPixmap(view_image_);
        painter.drawPixmap(QRect(350 + Dx, 25 + Dy, 600 - Dx, 600 - Dy), QPixmap(view_image_));
    }

private:
    QWidget* p;

    QImage img;

    int Dx = 0, Dy = 0;

};

Route_Tool::Route_Tool(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Route_Tool)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    this->setMouseTracking(true);
    ui->Route_View->setMouseTracking(true);
    ui->Axis_x->setStyleSheet("color: transparent");
    ui->Axis_y->setStyleSheet("color: transparent");
}

Route_Tool::~Route_Tool()
{
    delete ui;
}

void Route_Tool::on_pushButton_view_clicked()
{
    MainWindow* pa = (MainWindow*)parentWidget();

    QString Img_Dir = pro_path;
    preCentreX = ui->doubleSpinBox_centreX->value();
    preCentreY = ui->doubleSpinBox_centreY->value();

    Img_Dir = QCoreApplication::applicationDirPath() + "/temp";
    Create_Image_Task* route_tool_img = new Create_Image_Task(pa);

    QDir ck(QCoreApplication::applicationDirPath() + "/temp");
    if(!ck.exists())
    {
        ck.mkdir(ck.absolutePath());
    }

    if (!pa->createImagePre(route_tool_img))
    {
        delete route_tool_img;
        return;
    }

    if(pa->pre_info[pa->current_info_v].template_ == 2)
    {
        double t = ui->doubleSpinBox_t->value();
        std::complex<double> c1 = pa->pre_info[pa->current_info_v].Julia_c1, c2 = pa->pre_info[pa->current_info_v].Julia_c2;
        double k = pa->pre_info[pa->current_info_v].Julia_c_rate;
        route_tool_img->setTemplate2(c1 + (c2 - c1) * ((1 - k) * t + k * t * t));
    }
    if(pa->pre_info[pa->current_info_v].template_ == 4)
    {
        double t = ui->doubleSpinBox_t->value();

        double& k = pa->pre_info[pa->current_info_v].Newton_c_rate;

        std::complex<double> arr[10];
        for(int i = 0; i != 10; i++)
        {
            arr[i] = _curr_complex(pa->pre_info[pa->current_info_v].Newton_xn_1[i], pa->pre_info[pa->current_info_v].Newton_xn_2[i], t, k);
        }

        route_tool_img->setTemplate4(_curr_complex(pa->pre_info[pa->current_info_v].Newton_a_1, pa->pre_info[pa->current_info_v].Newton_a_2, t, k),
                                     arr,
                                     _curr_complex(pa->pre_info[pa->current_info_v].Newton_sin_1, pa->pre_info[pa->current_info_v].Newton_sin_2, t, k),
                                     _curr_complex(pa->pre_info[pa->current_info_v].Newton_cos_1, pa->pre_info[pa->current_info_v].Newton_cos_2, t, k),
                                     _curr_complex(pa->pre_info[pa->current_info_v].Newton_ex_1, pa->pre_info[pa->current_info_v].Newton_ex_2, t, k));

    }
    route_tool_img->setImage(ui->doubleSpinBox_centreX->value(), ui->doubleSpinBox_centreY->value(),
                             ui->doubleSpinBox_size->value(), ui->doubleSpinBox_size->value(), 600, 600,
                             -ui->doubleSpinBox_angle->value(), ui->doubleSpinBox_t->value(),
                             "png", Img_Dir, "Route_Tool Image", "Route",
                             pa->pre_info[pa->current_info_v].y_inverse);
    QThreadPool::globalInstance()->start(route_tool_img);
}

void Route_Tool::updateProgressBar(double p)
{
    ui->progressBar->setValue(p);
}

void Route_Tool::getImage(QImage img)
{
    qDebug() << "Preview get image";
    QPixmap view_image = QPixmap::fromImage(img);
    centreX = ui->doubleSpinBox_centreX->value();
    centreY = ui->doubleSpinBox_centreY->value();
    img_size = ui->doubleSpinBox_size->value();
    t = ui->doubleSpinBox_t->value();
    image = img;
    update();
}

void Route_Tool::mouseMoveEvent(QMouseEvent* e)
{
    if(e->position().x() >= 350 && e->position().x() <= 950
            && e->position().y() >= 25 && e->position().y() <= 625
            && ui->progressBar->value() == 100)
    {
        MainWindow* p = (MainWindow*)parent();
        double x_0 = static_cast<double>((e->position().x() - 650) / 600);
        double y_0 = -static_cast<double>((e->position().y() - 325) / 600);
        if (p->pre_info[p->current_info_v].y_inverse) y_0 = -y_0;
        if(e->buttons() & Qt::LeftButton)
        {
            press_move = true;
            dx = e->pos().x() - preX;
            dy = e->pos().y() - preY;
            double r = sqrt(dx * dx + dy * dy) * img_size / 600;
            double theta_0 = atan2(dy, dx);
            double theta = theta_0 + ui->doubleSpinBox_angle->value() * Pi / 180;
            // if (!p->pre_info[p->current_info_v].y_inverse) theta = theta_0 - ui->doubleSpinBox_angle->value() * Pi / 180;
            double x = -r * cos(theta) + preCentreX;
            double y = r * sin(theta) + preCentreY;
            if (p->pre_info[p->current_info_v].y_inverse) y = - r * sin(theta) + preCentreY;
            ui->doubleSpinBox_centreX->setValue(x);
            ui->doubleSpinBox_centreY->setValue(y);
            centreX = x;
            centreY = y;
            qDebug() << "Pressed moving";
            update();
        }
        else
        {
            double r = sqrt(x_0 * x_0 + y_0 * y_0) * img_size;
            double theta_0 = atan2(y_0, x_0);
            double theta = theta_0 - ui->doubleSpinBox_angle->value() * Pi / 180;
            // if (p->pre_info[p->current_info_v].y_inverse) theta = theta_0 + ui->doubleSpinBox_angle->value() * Pi / 180;
            double x = r * cos(theta) + centreX;
            double y = r * sin(theta) + centreY;
            ui->Label_Coordinate->setText(tr("(") + QString::number(x, ' ', 8) + ", " + QString::number(y, ' ', 8) + ")");
            preX = x_0 * 600 + 650;
            preY = -y_0 * 600 + 325;
            if (!p->pre_info[p->current_info_v].y_inverse) ;
            else preY = y_0 * 600 + 325;
        }
    }
    qDebug() << preX << "," << preY;
}

void Route_Tool::mouseReleaseEvent(QMouseEvent* e)
{
    if(!press_move) return;
    dx = dy = 0;
    on_pushButton_view_clicked();
    press_move = false;
}

void Route_Tool::mouseDoubleClickEvent(QMouseEvent* e)
{
    MainWindow* p = (MainWindow*)parent();
    if(e->button() == Qt::LeftButton && ui->progressBar->value() == 100)
    {
        if(e->position().x() >= 350 && e->position().y() <= 950
                && e->position().y() >= 25 && e->position().y() <= 625)
        {
            double x_0 = static_cast<double>((e->position().x() - 650) / 600) * img_size;
            double y_0 = -static_cast<double>((e->position().y() - 325) / 600) * img_size;
            if (p->pre_info[p->current_info_v].y_inverse) y_0 = -y_0;
            double r = sqrt(x_0 * x_0 + y_0 * y_0);
            double theta_0 = atan2(y_0, x_0);
            double theta = theta_0 + ui->doubleSpinBox_angle->value() * Pi / 180;
            // if (!p->pre_info[p->current_info_v].y_inverse) theta = theta_0 - ui->doubleSpinBox_angle->value() * Pi / 180;
            double x = r * cos(theta) + centreX;
            double y = r * sin(theta) + centreY;
            ui->doubleSpinBox_centreX->setValue(x);
            ui->doubleSpinBox_centreY->setValue(y);
            preCentreX = x;
            preCentreY = y;
            // update();
        }
    }
}

void Route_Tool::on_comboBox_axisColour_currentIndexChanged(int index)
{
    switch(index)
    {
    case 0:
        ui->Axis_x->setStyleSheet("color: transparent");
        ui->Axis_y->setStyleSheet("color: transparent");
        break;
    case 1:
        ui->Axis_x->setStyleSheet("color: black");
        ui->Axis_y->setStyleSheet("color: black");
        break;
    case 2:
        ui->Axis_x->setStyleSheet("color: white");
        ui->Axis_y->setStyleSheet("color: white");
        break;
    case 3:
        ui->Axis_x->setStyleSheet("color: red");
        ui->Axis_y->setStyleSheet("color: red");
        break;
    case 4:
        ui->Axis_x->setStyleSheet("color: green");
        ui->Axis_y->setStyleSheet("color: green");
        break;
    case 5:
        ui->Axis_x->setStyleSheet("color: blue");
        ui->Axis_y->setStyleSheet("color: blue");
        break;
    case 6:
        ui->Axis_x->setStyleSheet("color: yellow");
        ui->Axis_y->setStyleSheet("color: yellow");
        break;
    case 7:
        ui->Axis_x->setStyleSheet("color: gray");
        ui->Axis_y->setStyleSheet("color: gray");
        break;
    default:
        break;
    }
    update();
}

void Route_Tool::setPath(QString str)
{
    pro_path = str;
}

void Route_Tool::on_Slider_time_valueChanged(int value)
{
    ui->doubleSpinBox_t->setValue(value/1000000000.0);
}

void Route_Tool::on_doubleSpinBox_t_editingFinished()
{
    ui->Slider_time->setValue(1000000000 * ui->doubleSpinBox_t->value());
}

void Route_Tool::wheelEvent(QWheelEvent* e)
{
    if(preX < 350 || preX > 950 || preY < 25 || preY > 625) return;
    int t = e->angleDelta().y() / 120;
    if(t > 0)
    {
        double s = 1 / (1 + 0.1 * t);
        ui->doubleSpinBox_size->setValue(ui->doubleSpinBox_size->value() * s);
        qDebug() << "Wheel Up" << s;
    }
    else
    {
        double s = 1 - 0.1 * t;
        ui->doubleSpinBox_size->setValue(ui->doubleSpinBox_size->value() * s);
        qDebug() << "Wheel down" << s;
    }
    //update();
    //on_pushButton_view_clicked();
}

void Route_Tool::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    qDebug() << "paintEvent";
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPixmap view_image = QPixmap::fromImage(image);
    QTransform translation;
    translation.translate(dx, dy);
    painter.setTransform(translation);
    view_image.scroll(dx, dy, 0, 0, 600, 600);
    painter.drawPixmap(350, 25, 600 , 600 , QPixmap(view_image), dx, dy, 600, 600);
}

void Route_Tool::on_actionBack_to_main_window_triggered()
{
    close();
}

void Route_Tool::on_actionView_triggered()
{
    on_pushButton_view_clicked();
}

void Route_Tool::on_pushButton_Save_clicked()
{
    MainWindow* p = (MainWindow*)parentWidget();
    p->on_pushButton_routeAdd_clicked();
    p->table_route_model->item(p->table_route_model->rowCount() - 1, 0)->setData(QVariant(ui->doubleSpinBox_t->value()), Qt::EditRole);
    p->table_route_model->item(p->table_route_model->rowCount() - 1, 1)->setData(QVariant(ui->doubleSpinBox_centreX->value()), Qt::EditRole);
    p->table_route_model->item(p->table_route_model->rowCount() - 1, 2)->setData(QVariant(ui->doubleSpinBox_centreY->value()), Qt::EditRole);
    p->table_route_model->item(p->table_route_model->rowCount() - 1, 3)->setData(QVariant(ui->doubleSpinBox_angle->value()), Qt::EditRole);
    p->table_route_model->item(p->table_route_model->rowCount() - 1, 4)->setData(QVariant(ui->doubleSpinBox_size->value() * width_to_size), Qt::EditRole);
}

void Route_Tool::on_actionSave_Point_triggered()
{
    on_pushButton_Save_clicked();
}

void Route_Tool::on_action0_5_triggered()
{
    width_to_size = 0.5;
    ui->action0_5->setChecked(true);
    ui->action0_8->setChecked(false);
    ui->action1  ->setChecked(false);
    ui->action1_2->setChecked(false);
    ui->action1_5->setChecked(false);
    ui->action2_0->setChecked(false);
}

void Route_Tool::on_action0_8_triggered()
{
    width_to_size = 0.8;
    ui->action0_5->setChecked(false);
    ui->action0_8->setChecked(true);
    ui->action1  ->setChecked(false);
    ui->action1_2->setChecked(false);
    ui->action1_5->setChecked(false);
    ui->action2_0->setChecked(false);
}

void Route_Tool::on_action1_triggered()
{
    width_to_size = 1;
    ui->action0_5->setChecked(false);
    ui->action0_8->setChecked(false);
    ui->action1  ->setChecked(true);
    ui->action1_2->setChecked(false);
    ui->action1_5->setChecked(false);
    ui->action2_0->setChecked(false);
}

void Route_Tool::on_action1_2_triggered()
{
    width_to_size = 1.2;
    ui->action0_5->setChecked(false);
    ui->action0_8->setChecked(false);
    ui->action1  ->setChecked(false);
    ui->action1_2->setChecked(true);
    ui->action1_5->setChecked(false);
    ui->action2_0->setChecked(false);
}

void Route_Tool::on_action1_5_triggered()
{
    width_to_size = 1.5;
    ui->action0_5->setChecked(false);
    ui->action0_8->setChecked(false);
    ui->action1  ->setChecked(false);
    ui->action1_2->setChecked(false);
    ui->action1_5->setChecked(true);
    ui->action2_0->setChecked(false);
}

void Route_Tool::on_action2_0_triggered()
{
    width_to_size = 2;
    ui->action0_5->setChecked(false);
    ui->action0_8->setChecked(false);
    ui->action1  ->setChecked(false);
    ui->action1_2->setChecked(false);
    ui->action1_5->setChecked(false);
    ui->action2_0->setChecked(true);
}
