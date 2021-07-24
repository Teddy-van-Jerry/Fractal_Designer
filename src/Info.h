#ifndef INFO_H
#define INFO_H

#include <QString>
#include <QVector>
#include <QVariant>
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
    _FRD_CLASS_INT_,
    _FRD_CLASS_DOUBLE_,
    _FRD_CLASS_STRING_,
    _FRD_CLASS_BOOL_,
    _FRD_CLASS_FORMULA_,
    _FRD_CLASS_CUSTOM_ = -1
};

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
    // get member of the class
    virtual QVariant operator[](const QString& name) const = 0;

    virtual QVariant& operator[](const QString& name) = 0;

    QVariant operator[](int index) const;

    QVariant& operator[](int index);

    int index(const QString& name) const;

    bool contains(const QString& name) const;

    QStringList members() const;

protected:
    // list of all members of a class
    // need to be specified in constructor
    QStringList member_names;

    QVector<QVariant> member_vars;
};

class range_ : public frd_class_base_ {
public:
    // double From, To;

    FRD_var_ From { _FRD_CLASS_DOUBLE_, "From",  };

    QVariant operator[](const QString& name) const override;
    QVariant& operator[](const QString& name) override;
};

class color_ : public frd_class_base_  {
public:
    enum type_ {_RGB_, _HSV_} Type;
    QString R, G, B, H, S, V, A; // color formulas

    QVariant operator[](const QString& name) const override;
    QVariant& operator[](const QString& name) override;
};

class template_ : public frd_class_base_  {
public:
    QString Formula;
    double MIN, MAX;
    QString Compare = "abs($z)";
    bool YInverse = false;

    QVariant operator[](const QString& name) const override;
    QVariant& operator[](const QString& name) override;
};

class point_ : public frd_class_base_  {
public:
    double X, Y;

    QVariant operator[](const QString& name) const override;
    QVariant& operator[](const QString& name) override;
};

class path_point_ : public frd_class_base_  {
public:
    point_ Point;
    double T;
    double Rotation;

    QVariant operator[](const QString& name) const override;
    QVariant& operator[](const QString& name) override;
};

class path_ : public frd_class_base_  {
public:
    enum type_ {_FORMUALA_, _POINTS_} Type;
    QString X, Y, Width, Rotation;
    QVector<path_point_> Points;

    QVariant operator[](const QString& name) const override;
    QVariant& operator[](const QString& name) override;
};

class layer_ : public frd_class_base_  {
public:
    range_ Range;
    template_ Template;
    color_ Con, Div;
    path_ Path;

    QVariant operator[](const QString& name) const override;
    QVariant& operator[](const QString& name) override;
};

class music_ : public frd_class_base_  {
public:
    QVector<QString> List;
    QVector<double> From; /**< the beginning time of music */
    QVector<double> To;   /**< the ending time of music */
    double Rate;

    QVariant operator[](const QString& name) const override;
    QVariant& operator[](const QString& name) override;
};

class output_ : public frd_class_base_  {
public:
    int Time;
    int Fps;
    int Width;
    int Height;
    QString ImagePrefix, ImageDir, VideoName, VideoDir;

    QVariant operator[](const QString& name) const override;
    QVariant& operator[](const QString& name) override;
};

class config_ : public frd_class_base_  {
public:
    double PreviewX, PreviewY;
    double PreviewWidth, PreviewHeight;
    int PreviewImageWidth, PreviewImageHeight;
    bool AutoRefresh;

    QVariant operator[](const QString& name) const override;
    QVariant& operator[](const QString& name) override;
};

class custom_class_ {
public:
    QString className;
    int row, col;
};

enum error_type_ {
    _FRD_ERROR_UNEXPECTED_TOKEN_ = 1000,
    _FRD_ERROR_COMMENT_UNFINISHED_,
    _FRD_ERROR_OTHER_,
    _FRD_WARNING_DECLARATION_ONLY_,
    _FRD_WARNING_OTHER
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

class Info {
public:
    QVector<FRD> all;       /**< historic info */
    int i = 0;              /**< current info index */
    FRD editor;             /**< current info of editor */
};

#endif // INFO_H
