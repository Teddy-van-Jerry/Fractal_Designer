#ifndef CREATE_IMAGE_TASK_H
#define CREATE_IMAGE_TASK_H
#include <QRunnable>
#include <QWidget>
#include <QObject>
#include <QImage>
#include <complex>
#include <string>
#include "String_Complex.h"

#ifndef Create_Images_Task_Pre
#define Create_Images_Task_Pre(task__)       \
    task__->setData(post1,                   \
                    post2,                   \
                    curr_info.template_,     \
                    curr_info.min_class_v,   \
                    curr_info.max_class_v,   \
                    curr_info.max_loop_t);
#endif

class Create_Image_Task : public QObject, public QRunnable
{
    Q_OBJECT

public:
    Create_Image_Task(QWidget*);

    void run();

private:

    QString pro_path;

    bool Version_Higher_Than_4 = false;

    int template_, max_loop_t;

    // Template 2
    std::complex<double> c0;

    // Template 4
    std::complex<double> Newton_a = 1;
    std::complex<double> Newton_xn[10] {0};
    std::complex<double> Newton_sin = 0;
    std::complex<double> Newton_cos = 0;
    std::complex<double> Newton_ex = 0;

    std::vector<var> Colour1_f[4], Colour2_f[4];

    enum Colour_Type { RGBA } c_type;

    double min_class_v, max_class_v;

    double x, y, x_width, y_height, rotate_angle, t;
    int X, Y;
    QString img_format, img_path, img_title, work_name;

    bool isCancelled = false;

    int range_complex_to_255(const std::complex<double>& c);

    bool setRGBA(double[4], bool, const std::complex<double>&, const std::complex<double>&, double, int);

signals:
    void updateImage_preview(QImage);

    void progressInform_preview(double);

    void updateImage_route(QImage);

    void progressInform_route(double);

    void image_build_finished();

    void finished();

    void one_ok();

    void error_calc();

public: // function
    void setImage(double, double, double, double, int, int, double, double, QString, QString, QString, QString);

    void setData(std::vector<var>[4], std::vector<var>[4], int, double, double, int);

    void setTemplate2(std::complex<double> c);

    void setTemplate4(const std::complex<double>& c1, std::complex<double> c2[10], const std::complex<double>& c3,
                             const std::complex<double>& c4, const std::complex<double>& c5);

public slots:

    void stop();

};

#endif // CREATE_IMAGE_TASK_H
