#ifndef FRD_JSON_H
#define FRD_JSON_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDateTime>

enum FRD_error_type {
    _FRD_NO_ERROR_ = 0,
    _FRD_ERROR_UNEXPECTED_TOKEN_ = 1000,
    _FRD_ERROR_COMMENT_UNFINISHED_,
    _FRD_ERROR_UNDEFINED_VARIABLE_,
    _FRD_ERROR_ASSIGNMENT_NO_MATCH_,
    _FRD_ERROR_INVALID_STRICT_ASSIGNMENT_,
    _FRD_ERROR_WRONG_PARAPETER_TYPE_,
    _FRD_ERROR_NON_NUMBER_VALUE_,
    _FRD_ERROR_EVAL_,
    _FRD_ERROR_FUNCTION_SYNTAX_,
    _FRD_ERROR_OTHER_,
    _FRD_WARNING_VARIABLE_DECLARATION_ONLY_ = 8000,
    _FRD_WARNING_VARIABLE_REDEFINITION_,
    _FRD_WARNING_FUNCTION_DECLARATION_ONLY_,
    _FRD_WARNING_FUNCTION_REDEFINITION_,
    _FRD_WARNING_OTHER
};

struct FRD_Error {
    FRD_Error();
    FRD_Error(int type, int row, int length, int col, const QString& msg);
    FRD_error_type error_type;
    int row;
    int col;
    int length;
    QString msg;
};

class FRD_Json {
public:
    FRD_Json();

    // used for main
    QJsonValue operator[](const QString& name) const;

    QString type(const QString& block, const QString& name) const;

    QString type(const QString& block, const QStringList& names) const;

    QJsonValue getValue(const QString& block, const QString& base_name, const QString& var_name) const;

    QJsonValue getValue(const QString& block, const QStringList& base_names, const QStringList& var_names) const;

    bool contains(const QString& block, const QString& name) const;

    bool containsAbsolute(const QString& block, const QString& name);

    // set the value
    // if name does not exist, create it
    FRD_error_type setValue(const QString& block, const QString& base_name, const QString& var_name, const QString& type, QJsonValue value);

    // set the value
    // if name does not exist, check in the wider block
    FRD_error_type setExistantValue(const QString& block, const QString& base_name, const QString& var_name, const QString& type, QJsonValue value);

    FRD_error_type setExistantValue(const QString& block, const QStringList& base_names, const QStringList& var_names, const QString& type, QJsonValue value);

    // put the value into main
    FRD_error_type useValue(const QString& block, const QString& name);

    // add error information
    void addError(FRD_error_type error_type, const QString& msg, int row, int col, int length);

    QString toJson(QJsonDocument::JsonFormat = QJsonDocument::JsonFormat::Indented) const;

    QString varsToJson(QJsonDocument::JsonFormat = QJsonDocument::JsonFormat::Indented) const;

    void clear();

    QString text() const;

    QVector<FRD_Error> errors() const;

    QString layerName(int index) const;

    QString layerFormula(int index) const;

    QString layerColor(int index, QString tag) const;

    double PreviewSize(QString tag) const;

    double PreviewCentre(QString tag) const;

    int PreviewImageSize(QString tag) const;

    double PreviewRotation() const;

    double PreviewTime() const;

    bool inverseYAsis() const;

    QString distance(int index) const;

    QString templateMin(int index) const;

    QString templateMax(int index) const;

    QString iterationLimit(int index) const;

    double fps() const;

    QString imageDir() const;

    QString imagePrefix() const;

    int ImageSize(QString tag) const;

    int outputTime() const;

    QString videoDir() const;

    QString videoFormat() const;

    QString videoName() const;

    QDateTime runTime() const;

    QString runVersion() const;

public slots:
    void updateText(QString text);

private:
    QJsonObject main;  // main object of doc
    QJsonObject vars;  // key by their names

    QString frd_text;
};

#endif // FRD_JSON_H
