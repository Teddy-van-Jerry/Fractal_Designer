#include "FRD_Json.h"

FRD_Json::FRD_Json() {
    main.insert("Version", "6.0.3");
    main.insert("Time", QDateTime::currentDateTimeUtc().toString("yyyy/MM/dd hh:mm UTC"));
    main.insert("Layers", QJsonValue::Array);
    main.insert("Config", QJsonValue::Object);
    main.insert("Output", QJsonValue::Object);
    main.insert("Errors", QJsonValue::Array);
}

QJsonValue FRD_Json::operator[](const QString& name) const {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    if (names.size() == 0) return QJsonValue::Null;
    QJsonValue ret = QJsonValue::Null;
    getValue(name, ret);
    return ret;
}

bool FRD_Json::absoluteGetValue(const QString& name, QJsonValue& value) const {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    return absoluteGetValue(names, value);
}

bool FRD_Json::absoluteGetValue(const QStringList& names, QJsonValue& value) const {
    if (names.isEmpty()) return false;

    QJsonValue v = vars[names[0]];
    if (v.isNull()) return false;
    v = v.toObject()["OBJECT"];

    for (int i = 1; i != names.size(); i++) {
        v = v.toObject()[names[i]];
        if (v.isNull()) return false;
    }
    value = v;
    return true;
}

bool FRD_Json::getValue(const QString& name, QJsonValue& value) const {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    return getValue(names, value);
}

bool FRD_Json::getValue(const QStringList& names_, QJsonValue& value) const {
    QStringList names = names_;
    if (names.size() == 0) return false;
    while (!absoluteGetValue(names, value)) {
        if (names.size() < 2) return false; // can not find
        *(names.end() - 2) = names.last();
        names.pop_back();
    }
    return true;
}

bool FRD_Json::contains(const QString &name) const {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    if (names.size() == 0) return false;
    while (!containsAbsolute(names)) {
        if (names.size() < 2) return false; // can not find
        *(names.end() - 2) = names.last();
        names.pop_back();
    }
    return true;
}

bool FRD_Json::containsAbsolute(const QString& name) const {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    return containsAbsolute(names);
}

bool FRD_Json::containsAbsolute(const QStringList& names) const {
    if (names.isEmpty()) return false;

    QJsonValue v = vars[names[0]];
    if (v.isNull()) return false;
    v = v.toObject()["OBJECT"];

    for (int i = 1; i != names.size(); i++) {
        v = v.toObject()[names[i]];
        if (v.isNull()) return false;
    }

    return true;
}

FRD_error_type FRD_Json::setGlobalValue(const QString& name, const QString& type, QJsonValue value) {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    if (names.size() == 0) return _FRD_ERROR_UNDEFINED_VARIABLE_;
    while (!setValue(names, type, value)) {
        if (names.size() < 2) return _FRD_ERROR_UNDEFINED_VARIABLE_; // can not find
        *(names.end() - 2) = names.last();
        names.pop_back();
    }
    return _FRD_NO_ERROR_;
}

FRD_error_type FRD_Json::setValue(const QString& name, const QString& type, QJsonValue value) {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    return setValue(names, type, value);
}

FRD_error_type FRD_Json::setValue(const QStringList& names, const QString& type, QJsonValue value) {
    if (names.isEmpty()) return _FRD_ERROR_UNDEFINED_VARIABLE_;

    QJsonValueRef v = vars[names[0]];
    if (v.isNull()) {
        QJsonObject new_obj;
        new_obj.insert("OBJECT", QJsonValue::Object);
        new_obj.insert("TYPE", type);
        vars.insert(names[0], new_obj);
    }
    v = v.toObject()["OBJECT"];

    for (int i = 1; i != names.size(); i++) {
        v = v.toObject()[names[i]];
        if (v.isNull()) {
            v.toObject().insert(names[i], QJsonValue::Object);
            v = v = v.toObject()[names[i]];
        }
    }

    if (value.type() != QJsonValue::Undefined && value.type() == v.type()) {
        v = value;
        return _FRD_NO_ERROR_;
    }
    else {
        // Can not convert
        return _FRD_ERROR_ASSIGNMENT_NO_MATCH_;
    }
}

FRD_error_type FRD_Json::useValue(const QString& varName) {
    if (!vars.contains(varName)) return _FRD_ERROR_UNDEFINED_VARIABLE_;
    auto var = vars[varName].toObject();
    QString type_name = var["TYPE"].toString();
    if (type_name == "Layer" || type_name == "Error") {
        // add another
        main[type_name + "s"].toArray().append(var["OBJECT"].toObject());
        return _FRD_NO_ERROR_;
    }
    else {
        if (main.contains(type_name)) {
            main[type_name].toObject() = var["OBJECT"].toObject();
            return _FRD_NO_ERROR_;
        }
        else return _FRD_ERROR_WRONG_PARAPETER_TYPE_;
    }
}

void FRD_Json::addError(FRD_error_type error_type, const QString& msg, int row, int col, int length) {
    main["Errors"].toObject().insert("Type", error_type);
    main["Errors"].toObject().insert("Message", msg);
    main["Errors"].toObject().insert("Row", row);
    main["Errors"].toObject().insert("Col", col);
    main["Errors"].toObject().insert("Length", length);
}
