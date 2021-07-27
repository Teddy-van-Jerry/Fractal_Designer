#ifndef INFO_H
#define INFO_H

#include <QString>
#include <QVector>
#include <QVariant>
#include <complex>
#include "FRD_Json.h"

#if 0
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

class FRD_var_ {
public:
    FRD_class_ varClass;
    QString varName;
    QVariant value;
    int row, col;
};

class FRD_fun_ {
public:
    FRD_class_ funClass;
    QString funName;
    int row, col;
};

class frd_class_base_ {
public:

//    QVariant operator[](int index) const;

//    QVariant& operator[](int index);

    int index(const QString& name) const;

    bool contains(const QString& name) const;

    QStringList members() const;

protected:
    // list of all members of a class
    // need to be specified in constructor
    QStringList member_names;

    // QVector<QVariant> member_vars;
};

class range_ : public frd_class_base_ {
public:
    // double From, To;

    FRD_var_ From { _FRD_CLASS_DOUBLE_, "From",  };
};

class color_ : public frd_class_base_  {
public:
    enum type_ {_RGB_, _HSV_} Type;
    QString R, G, B, H, S, V, A; // color formulas
};

class template_ : public frd_class_base_  {
public:
    QString Formula;
    double MIN, MAX;
    QString Compare = "abs($z)";
    bool YInverse = false;
};

class point_ : public frd_class_base_  {
public:
    double X, Y;
};

class path_point_ : public frd_class_base_  {
public:
    point_ Point;
    double T;
    double Rotation;
};

class path_ : public frd_class_base_  {
public:
    enum type_ {_FORMUALA_, _POINTS_} Type;
    QString X, Y, Width, Rotation;
    QVector<path_point_> Points;
};

class layer_ : public frd_class_base_  {
public:
    range_ Range;
    template_ Template;
    color_ Con, Div;
    path_ Path;
};

class music_ : public frd_class_base_  {
public:
    QVector<QString> List;
    QVector<double> From; /**< the beginning time of music */
    QVector<double> To;   /**< the ending time of music */
    double Rate;
};

class output_ : public frd_class_base_  {
public:
    int Time;
    int Fps;
    int Width;
    int Height;
    QString ImagePrefix, ImageDir, VideoName, VideoDir;
};

class config_ : public frd_class_base_  {
public:
    double PreviewX, PreviewY;
    double PreviewWidth, PreviewHeight;
    int PreviewImageWidth, PreviewImageHeight;
    bool AutoRefresh;
};

class custom_class_ {
public:
    QString className;
    int row, col;
};

class error_ {
public:
    error_type_ errorType;
    int row, col;
    int length = 1;
};

class FRD_var_list_ : public QVector<FRD_var_> {
public:
    int index(const QString& name) const;

    bool contains(const QString& name) const;

    // No range check
    FRD_var_ operator[](const QString& name) const;

    // No range check
    FRD_var_& operator[](const QString& name);

    void addVar(const FRD_class_& cls, const QString& name, const QVariant& value, int row_, int col_);
};

class FRD {
public:
    QVector<layer_> Layers;
    music_ Music;   /**< global variable */
    output_ Output; /**< global variable */
    config_ Config; /**< global variable */
    FRD_var_list_ Vars;
    QVector<FRD_fun_> Funs;
    QVector<custom_class_> CustomClasses;
    QVector<error_> Errors; /**< error list */
};
#endif

class Info {
public:
    QVector<FRD_Json> all;  /**< historic info */
    int i = 0;              /**< current info index */
    FRD_Json editor;        /**< current info of editor */
};

#endif // INFO_H
