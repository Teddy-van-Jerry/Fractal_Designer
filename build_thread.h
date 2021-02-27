#ifndef BUILD_THREAD_H
#define BUILD_THREAD_H

#include <QThread>
#include <QLabel>
#include <QProgressBar>
#include <QString>
#include <QImage>
#include <QObject>
#include <QFile>
#include "Complex.h"

#ifndef Pi
#define Pi 3.14159265358979323846
#endif

class Build_Thread : public QObject {

    Q_OBJECT

    QString pro_path;

    bool Version_Higher_Than_4 = false;

    double Colour_Data_1[4][29][2], Colour_Data_2[4][29][2];
    double Colour_Data[2][4][29][2] = {0};

    int template_, max_loop_t;

    double min_class_v, max_class_v;

public:
    explicit Build_Thread(QObject* parent = 0);

signals:
    void updateImage_preview(QImage);

    void progressInform_preview(double);

    void updateImage_route(QImage);

    void progressInform_route(double);

    void image_build_finished();

    void finished();

    void one_ok();

public slots:
    void drawImage(double, double, double, double, int, int, double, double,
                                    QString, QString, QString, QString);
    void setPath(QString);

    void setData(double C1[4][29][2], double[4][29][2], int, double, double, int);

    void setVersion(bool);
};

#endif // BUILD_THREAD_H
