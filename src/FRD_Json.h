#ifndef FRD_JSON_H
#define FRD_JSON_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDateTime>

#if 0
enum FRD_class {
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
#endif

enum FRD_error_type {
    _FRD_NO_ERROR_ = 0,
    _FRD_ERROR_UNEXPECTED_TOKEN_ = 1000,
    _FRD_ERROR_COMMENT_UNFINISHED_,
    _FRD_ERROR_UNDEFINED_VARIABLE_,
    _FRD_ERROR_ASSIGNMENT_NO_MATCH_,
    _FRD_ERROR_WRONG_PARAPETER_TYPE_,
    _FRD_ERROR_NON_NUMBER_VALUE_,
    _FRD_ERROR_EVAL_,
    _FRD_ERROR_FUNCTION_SYNTAX_,
    _FRD_ERROR_OTHER_,
    _FRD_WARNING_VARIABLE_DECLARATION_ONLY_,
    _FRD_WARNING_VARIABLE_REDEFINITION_,
    _FRD_WARNING_FUNCTION_DECLARATION_ONLY_,
    _FRD_WARNING_FUNCTION_REDEFINITION_,
    _FRD_WARNING_OTHER
};

class FRD_Json {
public:
    FRD_Json();

    QJsonValue operator[](const QString& name) const;

    QString type(const QString& name) const;

    QString type(const QStringList& names) const;

//    QString absoluteType(const QString& name) const;

//    QString absoluteType(const QStringList& names) const;

    bool addBaseVar(const QString& name, const QString& class_name);

    bool absoluteGetValue(const QString& name, QJsonValue& value) const;

    bool absoluteGetValue(const QStringList& names, QJsonValue& value) const;

    bool getValue(const QString& name, QJsonValue& value) const;

    bool getValue(const QStringList& names, QJsonValue& value) const;

    // QJsonValueRef operator[](const QString& key);

    bool contains(const QString& name) const;

    bool contains(const QStringList& names) const;

    bool containsAbsolute(const QString& name) const;

    bool containsAbsolute(const QStringList& names) const;

    // set the value
    // if name does not exist, check in the wider block
    FRD_error_type setGlobalValue(const QString& name, const QString& type, QJsonValue value);

    // set the value
    // if name does not exist, create it
    FRD_error_type setValue(const QString& name, const QString& type, QJsonValue value, bool force = true);

    FRD_error_type setValue(const QStringList& names, const QString& type, QJsonValue value, bool force = true);

    // put the value into main
    FRD_error_type useValue(const QString& varName);

    // add error information
    void addError(FRD_error_type error_type, const QString& msg, int row, int col, int length);

    QString toJson(QJsonDocument::JsonFormat = QJsonDocument::JsonFormat::Indented) const;

    QString varsToJson(QJsonDocument::JsonFormat = QJsonDocument::JsonFormat::Indented) const;

private:
    QJsonObject main;  // main object of doc
    QJsonObject vars;  // key by their names
};

//QString json2string(QJsonValue type) {
//    if (type == QJsonValue::Array)     return "Array";
//    if (type == QJsonValue::Bool)      return "Bool";
//    if (type == QJsonValue::Double)    return "Double";
//    if (type == QJsonValue::Null)      return "Null";
//    if (type == QJsonValue::Object)    return "Object";
//    if (type == QJsonValue::String)    return "String";
//    if (type == QJsonValue::Undefined) return "Undefined";
//    return "Unknown";
//}

inline bool isComplexJsonValue(QJsonValue v) {
    return v.isObject() && v.toObject().contains("Real") && v.toObject().contains("Imag");
}

#endif // FRD_JSON_H
