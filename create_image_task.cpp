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

    qDebug() << "The working thread: " << QThread::currentThreadId();
    double progress = 0;
    int progress_now = 0;

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
            std::complex<double> this_point(x + mod * cos(theta),
                               y + mod * sin(theta));
            std::complex<double> z0(this_point), last_point(-this_point);
            bool convergent = true;
            int k = 0;
            for (k = 0; k < max_loop_t; k++)
            {
                if (abs(this_point) > max_class_v)
                {
                    convergent = false;
                    break;
                }
                else if (template_ != 4 && abs(this_point) < min_class_v)
                {
                    // convergent = true;
                    break;
                }
                else if (template_ == 4 && abs(this_point - last_point) / (abs(this_point)) < min_class_v)
                {
                    // convergent = true;
                    break;
                }
                else
                {
                    if (template_ == 4) last_point = this_point;
                    switch(template_)
                    {
                    case 1: this_point = this_point * this_point + z0; break;
                    case 2: this_point = this_point * this_point + c0; break;
                    case 3:
                    {
                        std::complex<double> c { fabs(this_point.real()), fabs(this_point.imag())};
                        this_point = c * c + z0; break;
                    }
                    case 4:
                    {
                        std::complex<double> p = 0, p_ = 0;
                        for(int i = 0; i != 10; i++)
                        {
                            p += Newton_xn[i] * pow(this_point, i);
                            p_ += Newton_xn[i] * pow(this_point, i - 1) * std::complex<double>(i);
                        }
                        p += Newton_ex * exp(this_point);
                        p += Newton_sin * sin(this_point);
                        p += Newton_cos * cos(this_point);
                        p_ += Newton_ex * exp(this_point);
                        p_ += Newton_sin * cos(this_point);
                        p_ -= Newton_cos * sin(this_point);
                        this_point = this_point - Newton_a * p / p_;
                    }
                    case 5:
                    {
                        std::vector<std::complex<double>> num_list
                        {
                            this_point, this_point.real(), this_point.imag(), z0, z0.real(), z0.imag(), t, double(k)
                        };
                        std::string msg;
                        this_point = eval_postorder(Formula, num_list, &msg);
                        if (!msg.empty())
                        {
                            // 0 indicates formula error
                            emit error_calc(0);
                        }
                        break;
                    }
                    default: break;
                    }
                }
            }

            double RGBA[4];
            if (!setRGBA(RGBA, convergent, this_point, z0, t, k))
            {
                // 1 indicates colour error in convergent point
                // 2 indicates colour error in divergent point
                emit error_calc(convergent ? 1 : 2);
            }
            else
            {
                image_build.setPixel(i, j, qRgba(RGBA[0], RGBA[1], RGBA[2], RGBA[3]));
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

void Create_Image_Task::setData(std::vector<_var> C1[4], std::vector<_var> C2[4], int temp, double min, double max, int lpt)
{
    for (int i = 0; i != 4; i++)
    {
        Colour1_f[i] = C1[i];
        Colour2_f[i] = C2[i];
    }
    template_   = temp;
    min_class_v = min < 1E-10 ? 1E-10 : min;
    max_class_v = max < 1E-10 ? 1E-10 : max;
    max_loop_t  = lpt;
}

void Create_Image_Task::setFormula(const std::vector<_var>& post)
{
    Formula = post;
}

void Create_Image_Task::setTemplate2(std::complex<double> c)
{
    c0 = c;
}

void Create_Image_Task::setTemplate4(const std::complex<double>& c1, std::complex<double> c2[10], const std::complex<double>& c3, const std::complex<double>& c4, const std::complex<double>& c5)
{
    Newton_a = c1;
    for(int i = 0; i != 10; i++) Newton_xn[i] = c2[i];
    Newton_sin = c3;
    Newton_cos = c4;
    Newton_ex = c5;
}

void Create_Image_Task::stop()
{
    isCancelled = true;
}

int Create_Image_Task::range_complex_to_255(const std::complex<double>& c)
{
    if (c.real() > 255) return 255;
    if (c.real() < 0) return 0;
    return c.real() + 0.5;
}

bool Create_Image_Task::setRGBA(double rgba[4], bool convergent, const std::complex<double>& z, const std::complex<double>& z0, double t, int k)
{
    std::string msg;
    std::vector<std::complex<double>> num_list { z, z.real(), z.imag(), z0, z0.real(), z0.imag(), t, double(k) };
    if (convergent)
    {
        for (int i = 0; i != 4; i++)
        {
            rgba[i] = range_complex_to_255(eval_postorder(Colour1_f[i], num_list, &msg));
            if (!msg.empty())
            {
                return false;
            }
        }
    }
    else
    {
        for (int i = 0; i != 4; i++)
        {
            rgba[i] = range_complex_to_255(eval_postorder(Colour2_f[i], num_list, &msg));
            if (!msg.empty())
            {
                return false;
            }
        }
    }
    return true;
}
