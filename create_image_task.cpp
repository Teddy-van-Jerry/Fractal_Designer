#include "create_image_task.h"
#include "mainwindow.h"

Create_Image_Task::Create_Image_Task(QWidget* parent)
{
    MainWindow* p = (MainWindow*) parent;
    connect(this, &Create_Image_Task::updateImage_preview,    p,                    &MainWindow::getImage);
    connect(this, &Create_Image_Task::progressInform_preview, p,                    &MainWindow::updateProgressBar);
    connect(this, &Create_Image_Task::finished,               p,                    &MainWindow::build_image_finished_deal);
    connect(this, &Create_Image_Task::one_ok,                 p,                    &MainWindow::build_image_one_ok);
    connect(this, &Create_Image_Task::updateImage_route,      p->route_tool_window, &Route_Tool::getImage);
    connect(this, &Create_Image_Task::progressInform_route,   p->route_tool_window, &Route_Tool::updateProgressBar);
    connect(p,    &MainWindow::createImageStop,               this,                 &Create_Image_Task::stop);
}

void Create_Image_Task::run()
{
    qDebug() << "Got it here";

    y = -y;

    if(!Version_Higher_Than_4) // low version
    {
        QFile Colour_saved_1(pro_path + "/Colour_Set_1.txt");
        if(Colour_saved_1.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in1(&Colour_saved_1);
            for(int j = 0; j != 4; j++)
            {
                for(int i = 0; i != 29; i++)
                {
                    in1 >> Colour_Data[0][j][i][0] >> Colour_Data[0][j][i][1];
                }
            }
            Colour_saved_1.close();
        }

        QFile Colour_saved_2(pro_path + "/Colour_Set_2.txt");
        if(Colour_saved_2.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in2(&Colour_saved_2);
            for(int j = 0; j != 4; j++)
            {
                for(int i = 0; i != 29; i++)
                {
                    in2 >> Colour_Data[1][j][i][0] >> Colour_Data[1][j][i][1];
                }
            }
            Colour_saved_2.close();
        }
    }

    qDebug() << "The working thread: " << QThread::currentThreadId();
    double progress = 0;
    int progress_now = 0;

    if(!Version_Higher_Than_4)
    {
        QFile Template_(pro_path + "/Template_Choice.txt");
        if(Template_.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in3(&Template_);
            in3 >> template_;
            Template_.close();
        }

        QFile Define_Value_(pro_path + "/Define_Value.txt");
        if(Define_Value_.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in4(&Define_Value_);
            in4 >> min_class_v >> max_class_v >> max_loop_t;
            Define_Value_.close();
        }
    }

//    qDebug() << QThread::currentThreadId();
    QImage image_build(X, Y, QImage::Format_ARGB32);
    for(int i = X - 1; i >= 0; i--)
    {
        if(isCancelled) return;
        progress = 100 * static_cast<double>(X - i) / X;
        if(static_cast<int>(progress) > progress_now)
        {
            if(work_name == "Preview") emit progressInform_preview(progress);
            if(work_name == "Route") emit progressInform_route(progress);
            progress_now = static_cast<int>(progress);
        }
        for(int j = Y - 1; j >= 0; j--)
        {
            double dx = x_width * (static_cast<double>(i) / X - 0.5);
            double dy = y_height * (static_cast<double>(j) / Y - 0.5);
            double mod = sqrt(dx * dx + dy * dy);
            double theta = atan2(dy, dx) + rotate_angle / 180 * Pi;
            Complex this_point(x + mod * cos(theta),
                               y + mod * sin(theta));
            Complex z0(this_point), last_point(-this_point);
            bool test = true;
            int k = 0;
            for (k = 0; k < max_loop_t; k++)
            {
                if ((double)this_point.modulus() > max_class_v)
                {
                    test = false;
                    break;
                }
                else if (template_ != 4 && (double)this_point.modulus() < min_class_v)
                {
                    // test = true;
                    break;
                }
                else if (template_ == 4 && (double)(this_point - last_point).modulus() / (double)(this_point.modulus()) < min_class_v)
                {
                    // test = true;
                    break;
                }
                else
                {
                    if (template_ == 4) last_point = this_point;
                    switch(template_)
                    {
                    case 1: this_point = (this_point ^ 2.0) + z0; break;
                    case 2: this_point = (this_point ^ 2.0) + c0; break;
                    case 3: this_point = (Complex(fabs(this_point.getReal()), fabs(this_point.getImaginary())) ^ 2.0) + z0; break;
                    case 4:
                    {
                        Complex p = 0, p_ = 0;
                        for(int i = 0; i != 10; i++)
                        {
                            p += Newton_xn[i] * (this_point ^ double(i));
                            p_ += Newton_xn[i] * (this_point ^ double(i - 1)) * Complex(i);
                        }
                        p += Newton_ex * exp(this_point);
                        p += Newton_sin * sin(this_point);
                        p += Newton_cos * cos(this_point);
                        p_ += Newton_ex * exp(this_point);
                        p_ += Newton_sin * cos(this_point);
                        p_ -= Newton_cos * sin(this_point);
                        this_point = this_point - Newton_a * p / p_;
                    }
                    default: break;
                    }
                }
            }

            if(test)
            {
                double RGBA1[4] = {0, 0, 0, 0};
                for(int m = 0; m != 4; m++)
                {
                    RGBA1[m] = (Colour_Data[0][m][0][0]  * t + Colour_Data[0][m][0][1])  * k +
                               (Colour_Data[0][m][1][0]  * t + Colour_Data[0][m][1][1])  * this_point.getReal() +
                               (Colour_Data[0][m][2][0]  * t + Colour_Data[0][m][2][1])  * this_point.getImaginary() +
                               (Colour_Data[0][m][3][0]  * t + Colour_Data[0][m][3][1])  * (double)this_point.modulus() +
                               (Colour_Data[0][m][4][0]  * t + Colour_Data[0][m][4][1])  * (double)this_point.modulus() / min_class_v +
                               (Colour_Data[0][m][5][0]  * t + Colour_Data[0][m][5][1])  * this_point.argz() +
                               (Colour_Data[0][m][6][0]  * t + Colour_Data[0][m][6][1])  * sin(this_point.argz()) +
                               (Colour_Data[0][m][7][0]  * t + Colour_Data[0][m][7][1])  * cos(this_point.argz()) +
                               (Colour_Data[0][m][8][0]  * t + Colour_Data[0][m][8][1])  * z0.getReal() +
                               (Colour_Data[0][m][9][0]  * t + Colour_Data[0][m][9][1])  * z0.getImaginary() +
                               (Colour_Data[0][m][10][0] * t + Colour_Data[0][m][10][1]) * (double)z0.modulus() +
                               (Colour_Data[0][m][11][0] * t + Colour_Data[0][m][11][1]) * (double)z0.modulus() / min_class_v +
                               (Colour_Data[0][m][12][0] * t + Colour_Data[0][m][12][1]) * z0.argz() +
                               (Colour_Data[0][m][13][0] * t + Colour_Data[0][m][13][1]) * sin(z0.argz()) +
                               (Colour_Data[0][m][14][0] * t + Colour_Data[0][m][14][1]) * (double)(this_point - z0).modulus() +
                               (Colour_Data[0][m][15][0] * t + Colour_Data[0][m][15][1]) * exp((k > 10) ? 10 : k) +
                               (Colour_Data[0][m][16][0] * t + Colour_Data[0][m][16][1]) * exp(log(10) * ((k > 10) ? 10 : k)) +
                               (Colour_Data[0][m][17][0] * t + Colour_Data[0][m][17][1]) * log(1 + (double)this_point.modulus()) +
                               (Colour_Data[0][m][18][0] * t + Colour_Data[0][m][18][1]) * log(1 + (double)z0.modulus()) +
                               (Colour_Data[0][m][19][0] * t + Colour_Data[0][m][19][1]) * log(1 + (double)(this_point - z0).modulus()) +
                               (Colour_Data[0][m][20][0] * t + Colour_Data[0][m][20][1]) * exp((double)this_point.modulus() > 10 ? 10 : (double)this_point.modulus() > 10) +
                               (Colour_Data[0][m][21][0] * t + Colour_Data[0][m][21][1]) * exp((double)z0.modulus() > 10 ? 10 : (double)z0.modulus()) +
                               (Colour_Data[0][m][22][0] * t + Colour_Data[0][m][22][1]) * exp((double)(this_point - z0).modulus() > 10 ? 10 : (double)this_point.modulus()) +
                               (Colour_Data[0][m][23][0] * t + Colour_Data[0][m][23][1]) * exp(log(10) * (double)this_point.modulus() > 10 ? 10 : log(10) * (double)this_point.modulus()) +
                               (Colour_Data[0][m][24][0] * t + Colour_Data[0][m][24][1]) * exp(log(10) * (double)z0.modulus() > 10 ? 10 : log(10) * (double)z0.modulus()) +
                               (Colour_Data[0][m][25][0] * t + Colour_Data[0][m][25][1]) * exp(log(10) * (double)(this_point - z0).modulus()) +
                               (Colour_Data[0][m][26][0] * t + Colour_Data[0][m][26][1]) * exp((double)this_point.modulus() / min_class_v > 10 ? 10 : (double)this_point.modulus() / min_class_v) +
                               (Colour_Data[0][m][27][0] * t + Colour_Data[0][m][27][1]) * exp((double)z0.modulus() / min_class_v > 10 ? 10 : (double)z0.modulus() / min_class_v) +
                               (Colour_Data[0][m][28][0] * t + Colour_Data[0][m][28][1]);
                    if(RGBA1[m] < 0) RGBA1[m] = 0;
                    else if(RGBA1[m] > 255) RGBA1[m] = 255;
                }
                image_build.setPixel(i, j, qRgba(RGBA1[0], RGBA1[1], RGBA1[2], RGBA1[3]));
            }
            else
            {
                double RGBA2[4];
                for(int m = 0; m != 4; m++)
                {
                    RGBA2[m] = (Colour_Data[1][m][0][0]  * t + Colour_Data[1][m][0][1])  * k +
                               (Colour_Data[1][m][1][0]  * t + Colour_Data[1][m][1][1])  * this_point.getReal() +
                               (Colour_Data[1][m][2][0]  * t + Colour_Data[1][m][2][1])  * this_point.getImaginary() +
                               (Colour_Data[1][m][3][0]  * t + Colour_Data[1][m][3][1])  * (double)this_point.modulus() +
                               (Colour_Data[1][m][4][0]  * t + Colour_Data[1][m][4][1])  * (double)this_point.modulus() / max_class_v +
                               (Colour_Data[1][m][5][0]  * t + Colour_Data[1][m][5][1])  * this_point.argz() +
                               (Colour_Data[1][m][6][0]  * t + Colour_Data[1][m][6][1])  * sin(this_point.argz()) +
                               (Colour_Data[1][m][7][0]  * t + Colour_Data[1][m][7][1])  * cos(this_point.argz()) +
                               (Colour_Data[1][m][8][0]  * t + Colour_Data[1][m][8][1])  * z0.getReal() +
                               (Colour_Data[1][m][9][0]  * t + Colour_Data[1][m][9][1])  * z0.getImaginary() +
                               (Colour_Data[1][m][10][0] * t + Colour_Data[1][m][10][1]) * (double)z0.modulus() +
                               (Colour_Data[1][m][11][0] * t + Colour_Data[1][m][11][1]) * (double)z0.modulus() / max_class_v +
                               (Colour_Data[1][m][12][0] * t + Colour_Data[1][m][12][1]) * z0.argz() +
                               (Colour_Data[1][m][13][0] * t + Colour_Data[1][m][13][1]) * sin(z0.argz()) +
                               (Colour_Data[1][m][14][0] * t + Colour_Data[1][m][14][1]) * (double)(this_point - z0).modulus() +
                               (Colour_Data[1][m][15][0] * t + Colour_Data[1][m][15][1]) * exp((k > 10) ? 10 : k) +
                               (Colour_Data[1][m][16][0] * t + Colour_Data[1][m][16][1]) * exp(log(10) * ((k > 10) ? 10 : k)) +
                               (Colour_Data[1][m][17][0] * t + Colour_Data[1][m][17][1]) * log(1 + (double)this_point.modulus()) +
                               (Colour_Data[1][m][18][0] * t + Colour_Data[1][m][18][1]) * log(1 + (double)z0.modulus()) +
                               (Colour_Data[1][m][19][0] * t + Colour_Data[1][m][19][1]) * log(1 + (double)(this_point - z0).modulus()) +
                               (Colour_Data[1][m][20][0] * t + Colour_Data[1][m][20][1]) * exp((double)this_point.modulus() > 10 ? 10 : (double)this_point.modulus() > 10) +
                               (Colour_Data[1][m][21][0] * t + Colour_Data[1][m][21][1]) * exp((double)z0.modulus() > 10 ? 10 : (double)z0.modulus()) +
                               (Colour_Data[1][m][22][0] * t + Colour_Data[1][m][22][1]) * exp((double)(this_point - z0).modulus() > 10 ? 10 : (double)this_point.modulus()) +
                               (Colour_Data[1][m][23][0] * t + Colour_Data[1][m][23][1]) * exp(log(10) * (double)this_point.modulus() > 10 ? 10 : log(10) * (double)this_point.modulus()) +
                               (Colour_Data[1][m][24][0] * t + Colour_Data[1][m][24][1]) * exp(log(10) * (double)z0.modulus() > 10 ? 10 : log(10) * (double)z0.modulus()) +
                               (Colour_Data[1][m][25][0] * t + Colour_Data[1][m][25][1]) * exp(log(10) * (double)(this_point - z0).modulus()) +
                               (Colour_Data[1][m][26][0] * t + Colour_Data[1][m][26][1]) * exp((double)this_point.modulus() / max_class_v > 10 ? 10 : (double)this_point.modulus() / max_class_v) +
                               (Colour_Data[1][m][27][0] * t + Colour_Data[1][m][27][1]) * exp((double)z0.modulus() / max_class_v > 10 ? 10 : (double)z0.modulus() / max_class_v) +
                               (Colour_Data[1][m][28][0] * t + Colour_Data[1][m][28][1]);
                    if(RGBA2[m] < 0) RGBA2[m] = 0;
                    else if(RGBA2[m] > 255) RGBA2[m] = 255;
                }
                image_build.setPixel(i, j, qRgba(RGBA2[0], RGBA2[1], RGBA2[2], RGBA2[3]));
            }
        }
    }
    qDebug() << img_path + "/" + img_title + "." + img_format;
    image_build.save(img_path + "/" + img_title + "." + img_format);
    image_build.load(img_path + "/" + img_title + "." + img_format);

    if(work_name == "Preview") emit updateImage_preview(image_build);
    if(work_name == "Route") emit updateImage_route(image_build);
    if(work_name == "Create_Image") emit one_ok();
    if(work_name == "Create_Image_Last") emit finished();

    // delete this;
}

void Create_Image_Task::setImage(double x_, double y_, double x_width_, double y_height_, int X_, int Y_, double rotate_angle_, double t_,
                                 QString img_format_, QString img_path_, QString img_title_, QString work_name_)
{
    x            = x_;
    y            = y_;
    x_width      = x_width_;
    y_height     = y_height_;
    X            = X_;
    Y            = Y_;
    rotate_angle = rotate_angle_;
    t            = t_;
    img_format   = img_format_;
    img_path     = img_path_;
    img_title    = img_title_;
    work_name    = work_name_;
}

void Create_Image_Task::setPath(QString str)
{
    pro_path = str;
}

void Create_Image_Task::setData(double C1[4][29][2], double C2[4][29][2], int temp, double min, double max, int lpt)
{
    For_All_Colour(i, j)
        Colour_Data[0][i][j][0] = C1[i][j][0];
        Colour_Data[0][i][j][1] = C1[i][j][1];
        Colour_Data[1][i][j][0] = C2[i][j][0];
        Colour_Data[1][i][j][1] = C2[i][j][1];
    End_All_Colour

    template_   = temp;
    min_class_v = min < 1E-10 ? 1E-10 : min;
    max_class_v = max < 1E-10 ? 1E-10 : max;
    max_loop_t  = lpt;
}

void Create_Image_Task::setTemplate2(const Complex& c)
{
    c0 = c;
}

void Create_Image_Task::setTemplate4(const Complex& c1, Complex* c2, const Complex& c3, const Complex& c4, const Complex& c5)
{
    Newton_a = c1;
    for(int i = 0; i != 10; i++) Newton_xn[i] = c2[i];
    Newton_sin = c3;
    Newton_cos = c4;
    Newton_ex = c5;
}

void Create_Image_Task::setVersion(bool v)
{
    Version_Higher_Than_4 = v;
}

void Create_Image_Task::stop()
{
    isCancelled = true;
}
