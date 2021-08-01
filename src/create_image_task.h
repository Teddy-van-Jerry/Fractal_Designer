#ifndef CREATE_IMAGE_TASK_H
#define CREATE_IMAGE_TASK_H
#include <QRunnable>
#include <QWidget>
#include <QObject>
#include <QImage>
#include <complex>
#include <string>
#include "String_Evaluate.h"

#ifndef Create_Images_Task_Pre
#define Create_Images_Task_Pre(task__) \
    task__->setData(postFormula,       \
                    post1,             \
                    post2,             \
                    postDistance,      \
                    postMin,           \
                    postMax,           \
                    postIterLimit)

#endif

class Create_Image_Task : public QObject, public QRunnable
{
    Q_OBJECT

public:

    enum Eval_Type {
        EVAL_FORMULA,
        EVAL_CON,
        EVAL_DIV,
        EVAL_DISTANCE,
        EVAL_MIN,
        EVAL_MAX,
        EVAL_ITER,
    };

    Create_Image_Task(QWidget*);

    void run();

private:

    QString pro_path;

    bool Version_Higher_Than_4 = false;

    std::vector<_var> distance_, max_loop_t;

    // Template 2
    std::complex<double> c0;

    // Template 4
    std::complex<double> Newton_a = 1;
    std::complex<double> Newton_xn[10] {0};
    std::complex<double> Newton_sin = 0;
    std::complex<double> Newton_cos = 0;
    std::complex<double> Newton_ex = 0;

    std::vector<_var> Colour1_f[4], Colour2_f[4], Formula;

    enum Colour_Type { RGBA } c_type;

    std::vector<_var> min_class_v, max_class_v;

    double x, y, x_width, y_height, rotate_angle, t;
    int X, Y;
    QString img_format, img_path, img_title, work_name;

    bool isCancelled = false;

    bool y_inverse = false;

    int range_complex_to_255(const std::complex<double>& c);

    bool setRGBA(double[4], bool, std::vector<std::complex<double>> num_list);

    std::complex<double> evalExpr(const std::vector<_var>& expr,
                                  const std::vector<std::complex<double>>& num_list, Eval_Type type, bool* ok = nullptr);

//    template <typename T>
//    bool evalExpr(T& val, std::vector<_var> expr,
//                  std::vector<std::complex<double>> num_list, Eval_Type type);

signals:
    void updateImage_preview(QImage);

    void progressInform_preview(double);

    void updateImage_route(QImage);

    void progressInform_route(double);

    void image_build_finished();

    void finished();

    void one_ok();

    void error_calc(int);

public: // function
    void setImage(double, double, double, double, int, int, double, double, QString, QString, QString, QString, bool);

    void setData(const std::vector<_var>& formula, const std::vector<_var> C1[4], const std::vector<_var> C2[4],
                 const std::vector<_var>& distance, const std::vector<_var>& min, const std::vector<_var>& max,
                 const std::vector<_var>& lpt);

    void setTemplate2(std::complex<double> c);

    void setTemplate4(const std::complex<double>& c1, std::complex<double> c2[10], const std::complex<double>& c3,
                             const std::complex<double>& c4, const std::complex<double>& c5);

    void setFormula(const std::vector<_var>&);
public slots:

    void stop();

};

#endif // CREATE_IMAGE_TASK_H
