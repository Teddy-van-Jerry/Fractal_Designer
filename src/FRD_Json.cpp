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

QString FRD_Json::type(const QString &name) const {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    return type(names);
}

QString FRD_Json::type(const QStringList& names) const {
    if (names.isEmpty()) return "Null";
    else if (names.size() == 1) {
        QJsonValue v = vars[names[0]];
        if (v.isNull()) return "Null";
        return v.toObject()["TYPE"].toString();
    }
    else {
        QString member = names.last();
        if (QVector<QString>({"Template"}).contains(member))
            return "Template";
        else if (QVector<QString>({"Con", "Div"}).contains(member))
            return "Color";
        else if (QVector<QString>({"R", "G", "B", "H", "S", "V", "A", "X", "Y"}).contains(member))
            return "Formula";
        else if (QVector<QString>({"VideoDir"}).contains(member))
            return "String";
        else if (QVector<QString>({"Fps", "Time"}).contains(member))
            return "Number";
        else {
            // TODO
            return "";
        }
    }
}

bool FRD_Json::addBaseVar(const QString &name, const QString &class_name){
    // must ensure that not the same name
    // only the first part of name will be used.
    qDebug() << "set base var" << name << class_name;
    QJsonObject obj;
    // if (class_name == "number") {
        obj.insert("TYPE", "Number");
        obj.insert("OBJECT", QJsonValue::Object);
    /* }
    else if (class_name == "complex") {
        obj.insert("TYPE", "Complex");
        QJsonObject complex_;
        complex_.insert("Real", 0);
        complex_.insert("Imag", 0);
        obj.insert("OBJECT", complex_);
    }
    else if (class_name == "layer") {
        obj.insert("TYPE", "Layer");
        obj.insert("OBJECT", QJsonValue::Object);
    }
    else if (class_name == "template") {
        obj.insert("TYPE", "Template");
        obj.insert("OBJECT", QJsonValue::Object);
    }
    else if (class_name == "color") {
        obj.insert("TYPE", "color");
        obj.insert("OBJECT", QJsonValue::Object);
    }
    else if (class_name == "formula") {
        obj.insert("TYPE", "Formula");
        obj.insert("OBJECT", "");
    }
    else return false;*/

    vars.insert(name, obj);
    return true;
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
        if (v.isNull()) return false; // value remains unchanged
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
/*
FRD_error_type FRD_Json::setGlobalValue(const QString& name, const QString& type, QJsonValue value) {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    if (names.size() == 0) return _FRD_ERROR_UNDEFINED_VARIABLE_;
    while (setValue(names, type, value, false) != _FRD_NO_ERROR_) {
        if (names.size() < 2) return _FRD_ERROR_UNDEFINED_VARIABLE_; // can not find
        *(names.end() - 2) = names.last();
        names.pop_back();
    }
    return _FRD_NO_ERROR_;
}

//FRD_error_type FRD_Json::setValue(const QString& name, const QString& type, QJsonValue value, bool force) {
//    QStringList names = name.split('.', Qt::SkipEmptyParts);
//    return setValue(names, type, value, force);
//}

FRD_error_type FRD_Json::setValue(const QStringList& names, const QString& type, QJsonValue value, bool force) {
    if (names.isEmpty()) return _FRD_ERROR_UNDEFINED_VARIABLE_;

    QJsonValueRef v = vars[names[0]];
    if (v.isNull()) {
        if (force) {
            addBaseVar(names[0], type);
            v = vars[names[0]];
            qDebug() << "After addBaseVar" << v;
        }
        else return _FRD_ERROR_UNDEFINED_VARIABLE_;
    }
    v = v.toObject()["OBJECT"];

    for (int i = 1; i != names.size(); i++) {
        v = v.toObject()[names[i]];
        if (v.isNull()) {
            if (force) {
                v.toObject().insert(names[i], QJsonValue::Object);
                v = v = v.toObject()[names[i]];
                qDebug() << "Insert new to object" << vars;
            }
            else {
                return _FRD_ERROR_UNDEFINED_VARIABLE_;
            }
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
*/
FRD_error_type FRD_Json::setValue(const QString& block, const QString& name, const QString& type, QJsonValue value) {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    if (names.empty()) return _FRD_ERROR_UNDEFINED_VARIABLE_; // change into empty variable name
    QString key = block + names[0];

    QVector<QJsonValueRef> refs;
    if (vars.contains(key)) {
        refs.push_back(vars[key].toObject()["OBJECT"]);
    }
    else {
        QJsonObject obj;
        obj.insert("TYPE", type);
        obj.insert("OBJECT", QJsonValue::Object);
        vars.insert(key, obj);
        refs.push_back(vars[key].toObject()["OBJECT"]);
    }
    for (int i = 1; i != names.size(); i++) {
        if (refs.last().toObject().contains(names[i])) {
            refs.push_back(refs.last().toObject()[names[i]]);
        }
        else {
            QJsonObject obj = refs.last().toObject();
            obj.insert(names[i], QJsonValue::Object);
            refs.last() = obj;
            refs.push_back(refs.last().toObject()[names[i]]);
        }
    }
    refs.last() = value;
    return _FRD_NO_ERROR_;
}

FRD_error_type FRD_Json::setExistedValue(const QString& block, const QString& name, const QString&, QJsonValue value) {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    if (names.empty()) return _FRD_ERROR_UNDEFINED_VARIABLE_; // change into empty variable name

    QString block_ = block;
    while(!vars.contains(block_ + names[0])) {
        do { block_.chop(1); } while(!block_.isEmpty() && block_.last(1) != ".");
        if (block_.isEmpty()) return _FRD_ERROR_UNDEFINED_VARIABLE_;
    }
    QVector<QJsonValueRef> refs;
    refs.push_back(vars[block_ + names[0]].toObject()["OBJECT"]);
    for (int i = 1; i != names.size(); i++) {
        if (refs.last().toObject().contains(names[i])) {
            refs.push_back(refs.last().toObject()[names[i]]);
        }
        else {
            return _FRD_ERROR_UNDEFINED_VARIABLE_;
        }
    }
    refs.last() = value;
    return _FRD_NO_ERROR_;
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

QString FRD_Json::toJson(QJsonDocument::JsonFormat format) const {
    QJsonDocument doc(main);
    return doc.toJson(format);
}

QString FRD_Json::varsToJson(QJsonDocument::JsonFormat format) const {
    QJsonDocument doc(vars);
    return doc.toJson(format);
}
