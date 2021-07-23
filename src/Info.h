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
    QVector<double> From; /**< the beginning time of music */
    QVector<double> To;   /**< the ending time of music */
    double Rate;
};

class output_ {
public:
    int Time;
    int Fps;
    int Width;
    int Height;
    QString ImagePrefix, ImageDir, VideoName, VideoDir;
};

class config_ {
public:
    double PreviewX, PreviewY;
    double PreviewWidth, PreviewHeight;
    int PreviewImageWidth, PreviewImageHeight;
    bool AutoRefresh;
};

enum FRD_class_ {
    _FRD_CLASS_LAYER_,
    _FRD_CLASS_MUSIC_,
    _FRD_CLASS_TEMPLATE_,
    _FRD_CLASS_COLOR_,
    _FRD_CLASS_PATH_,
    _FRD_CLASS_PATH_POINT_,
    _FRD_CLASS_POINT_,
    _FRD_CLASS_CONFIG_,
    _FRD_CLASS_OUTPUT_,
    _FRD_CLASS_RANGE_,
    _FRD_CLASS_CUSTOM_ = -1
};

class FRD_var_ {
public:
    FRD_class_ varClass;
    QString varName;
    int row, col;
};

class custom_class_ {
public:
    QString className;
    int row, col;
};

enum error_type_ {
    _FRD_ERROR_COMMENT_UNFINISHED_,
    _FRD_ERROR_OTHER_ = 9000
};

class error_ {
public:
    error_type_ errorType;
    int row, col;
};

class FRD {
public:
    QVector<layer_> Layers;
    music_ Music;   /**< global variable */
    output_ Output; /**< global variable */
    config_ Config; /**< global variable */
    QVector<FRD_var_> Vars;
    QVector<custom_class_> CustomClasses;
    QVector<error_> Errors; /**< error list */
};

class Info {
public:
    QVector<FRD> all;       /**< historic info */
    int i = 0;              /**< current info index */
    FRD editor;             /**< current info of editor */
};

#endif // INFO_H
