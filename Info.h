#ifndef INFO_H
#define INFO_H

#include <QString>
#include <QVector>
#include <complex>

class FRD;
class layer_;
class music_;
class template_;
class color_;
class path_;
class path_point_;
class point_;
class config_;
class output_;
class range_;

class range_ {
public:
    double From, To;
};

class color_ {
public:
    enum type_ {_RGB_, _HSV_} Type;
    QString R, G, B, H, S, V, A; // color formulas
};

class template_ {
public:
    QString Formula;
    double MIN, MAX;
    QString Compare = "abs($z)";
    bool YInverse = false;
};

class point_ {
public:
    double X, Y;
};

class path_point_ {
public:
    point_ Point;
    double T;
    double Rotation;
};

class path_ {
public:
    enum type_ {_FORMUALA_, _POINTS_} Type;
    QString X, Y, Width, Rotation;
    QVector<path_point_> Points;
};

class layer_ {
public:
    range_ Range;
    template_ Template;
    color_ Con, Div;
    path_ Path;
};

class music_ {
public:
    QVector<QString> List;
    QVector<double> From, To; // the beginning and ending time of music
    double Rate;
};

class output_ {
public:
    size_t Time;
    size_t Fps;
    size_t Width;
    size_t Height;
    QString ImagePrefix, ImageDir, VideoName, VideoDir;
};

class config_ {
public:
    double PreviewX, PreviewY;
    double PreviewWidth, PreviewHeight;
    size_t PreviewImageWidth, PreviewImageHeight;
    bool AutoRefresh;
};

class FRD {
public:
    QVector<layer_> Layers;
    music_ Music;   // global variable
    output_ Output; // global variable
    config_ Config; // global variable
};

class Info {
public:
    QVector<FRD> all; // historical info
    int i = 0;        // current info index
};

#endif // INFO_H
