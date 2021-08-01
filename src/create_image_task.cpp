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

    qDebug() << "The working thread: " << QThread::currentThreadId();
    double progress = 0;
    int progress_now = 0;

    std::complex<double> iterLimit = 1;
    std::complex<double> Min = 1000000000, Max = 1000000000;

    // If they are constant,
    // this will save the effort of calculation
    bool isIterLimitConst, isMinConst, isMaxConst;
    iterLimit = evalExpr(max_loop_t, {}, EVAL_ITER, &isIterLimitConst);
    Min       = evalExpr(min_class_v, {}, EVAL_MIN, &isMinConst);
    Max       = evalExpr(max_class_v, {}, EVAL_MAX, &isMaxConst);

    QImage image_build(X, Y, QImage::Format_ARGB32);
    for(int i = X - 1; i >= 0; i--)
    {
        if(isCancelled) return;
        progress = 100 * static_cast<double>(X - i) / X;
        if(static_cast<int>(progress) > progress_now)
        {
            if (work_name == "Preview") emit progressInform_preview(progress);
            else if (work_name == "Route") emit progressInform_route(progress);
            progress_now = static_cast<int>(progress);
        }
        for(int j = Y - 1; j >= 0; j--)
        {
            double dx = x_width * (static_cast<double>(i) / X - 0.5);
            double dy = - y_height * (static_cast<double>(j) / Y - 0.5);
            if (y_inverse) dy = -dy;
            double mod = sqrt(dx * dx + dy * dy);
            double theta = atan2(dy, dx) + rotate_angle / 180 * Pi;
            std::complex<double> this_point(x + mod * cos(theta), y + mod * sin(theta));
            std::complex<double> z0(this_point), last_point(-this_point);
            bool convergent = true;
            int k = 0;
            std::vector<std::complex<double>> num_list = {
                this_point, this_point.real(), this_point.imag(),
                last_point, last_point.real(), last_point.imag(),
                z0, z0.real(), z0.imag(), t, double(k)
            };

            for (k = 0; k < iterLimit.real() && k < 1000000000; k++)
            {
                num_list[3] = num_list[0];
                num_list[4] = num_list[1];
                num_list[5] = num_list[2];
                num_list[6] = z0;
                num_list[7] = z0.real();
                num_list[8] = z0.imag();
                num_list[9] = t;
                num_list[10] = k;
                num_list[0] = this_point;
                num_list[1] = this_point.real();
                num_list[2] = this_point.imag();
                bool ok;
                if (!isIterLimitConst)
                {
                    iterLimit = evalExpr(max_loop_t, num_list, EVAL_ITER, &ok);
                    if (!ok) break;
                }
                if (!isMinConst)
                {
                    Min = evalExpr(min_class_v, num_list, EVAL_MIN, &ok);
                    if (!ok) break;
                }
                if (!isMaxConst)
                {
                    Max = evalExpr(max_class_v, num_list, EVAL_MAX, &ok);
                    if (!ok) break;
                }
                // qDebug() << iterLimit.real() << Min.real() << Max.real();
                if (abs(this_point) < Min.real())
                {
                    break;
                }
                else if (abs(this_point) > Max.real())
                {
                    convergent = false;
                    break;
                }
                else
                {
                    this_point = evalExpr(Formula, num_list, EVAL_FORMULA, &ok);
                    if (!ok) break;
                }
            }

            double RGBA[4];
            if (setRGBA(RGBA, convergent, num_list))
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
                                 QString img_format_, QString img_path_, QString img_title_, QString work_name_, bool y_inverse_)
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
    y_inverse    = y_inverse_;
}

void Create_Image_Task::setData(const std::vector<_var>& formula, const std::vector<_var> C1[4], const std::vector<_var> C2[4],
                                const std::vector<_var>& distance, const std::vector<_var>& min, const std::vector<_var>& max,
                                const std::vector<_var>& lpt)
{
    for (int i = 0; i != 4; i++)
    {
        Colour1_f[i] = C1[i];
        Colour2_f[i] = C2[i];
    }
    Formula     = formula;
    distance_   = distance;
    min_class_v = min;
    max_class_v = max;
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
    for (int i = 0; i != 10; i++) Newton_xn[i] = c2[i];
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

bool Create_Image_Task::setRGBA(double rgba[4], bool convergent, std::vector<std::complex<double>> num_list)
{
    bool ok;
    if (convergent)
    {
        for (int i = 0; i != 4; i++)
        {
            std::complex<double> temp;
            temp = evalExpr(Colour1_f[i], num_list, EVAL_CON, &ok);
            if (!ok) return false;
            else rgba[i] = range_complex_to_255(temp);
        }
    }
    else
    {
        for (int i = 0; i != 4; i++)
        {
            std::complex<double> temp;
            temp = evalExpr(Colour2_f[i], num_list, EVAL_DIV, &ok);
            if (!ok) return false;
            else rgba[i] = range_complex_to_255(temp);
        }
    }
    return true;
}

std::complex<double> Create_Image_Task::evalExpr(const std::vector<_var>& expr,
                                                 const std::vector<std::complex<double>>& num_list, Eval_Type type, bool* ok)
{
    std::string msg;
    auto val = eval_postorder(expr, num_list, &msg);
    if (!msg.empty())
    {
        emit error_calc(type);
        if (ok) *ok = false;
    }
    else if (ok) *ok = true;
    return val;
}
