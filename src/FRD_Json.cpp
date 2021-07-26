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
    QJsonValue ret = main;
    for (int i = 0; i != names.size(); i++) {
        ret = ret[names[i]];
    }
    return ret;
}

QString FRD_Json::type(const QString& block, const QString &name) const {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    return type(block, names);
}

QString FRD_Json::type(const QString& block, const QStringList& names) const {
    if (names.isEmpty()) return "null";
    else if (names.size() == 1) {
        QJsonValue v = vars[block + names[0]];
        if (v.isNull()) return "null";
        else return v.toObject()["TYPE"].toString();
    }
    else {
        QString member = names.last();
        if (QVector<QString>({"Template"}).contains(member))
            return "template";
        else if (QVector<QString>({"Con", "Div"}).contains(member))
            return "color";
        else if (QVector<QString>({"R", "G", "B", "H", "S", "V", "A", "X", "Y"}).contains(member))
            return "formula";
        else if (QVector<QString>({"VideoDir"}).contains(member))
            return "string";
        else if (QVector<QString>({"Fps", "Time"}).contains(member))
            return "number";
        else {
            // TODO
            return "";
        }
    }
}

QJsonValue FRD_Json::getValue(const QString& block, const QString& name) const {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    return getValue(block, names);
}

QJsonValue FRD_Json::getValue(const QString& block, const QStringList& names_) const {
    QStringList names = names_;
    if (names.size() == 0) return QJsonValue::Null;

    QString block_ = block;
    while(!vars.contains(block_ + names[0])) {
        do { block_.chop(1); } while(!block_.isEmpty() && block_.last(1) != ".");
        if (block_.isEmpty()) return QJsonValue::Null;
    }
    QJsonValue value;
    value = vars[block_ + names[0]].toObject()["OBJECT"];
    for (int i = 1; i != names.size(); i++) {
        if (value.toObject().contains(names[i])) value = value.toObject()[names[i]];
        else return QJsonValue::Null;
    }
    return value;
}

bool FRD_Json::contains(const QString& block, const QString &name) const {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    if (names.size() == 0) return false;

    QString block_ = block;
    while(!vars.contains(block_ + names[0])) {
        do { block_.chop(1); } while(!block_.isEmpty() && block_.last(1) != ".");
        if (block_.isEmpty()) return false;
    }
    QJsonValue value;
    value = vars[block_ + names[0]].toObject()["OBJECT"];
    for (int i = 1; i != names.size(); i++) {
        if (value.toObject().contains(names[i])) value = value.toObject()[names[i]];
        else return false;
    }
    return true;
}

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
        if (type.isEmpty()) return _FRD_ERROR_OTHER_;
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

FRD_error_type FRD_Json::setExistantValue(const QString& block, const QString& name, const QString&, QJsonValue value) {
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

FRD_error_type FRD_Json::useValue(const QString& block, const QString& name) {
    qDebug() << "Entrance of useValue";
    if (!contains(block, name)) return _FRD_ERROR_UNDEFINED_VARIABLE_;
    auto var_value = getValue(block, name);
    auto var_type = type(block, name);
    qDebug() << "useValue var_type:" << var_type;
    if (var_type.isEmpty()) return _FRD_ERROR_UNDEFINED_VARIABLE_; // no type
    else var_type[0] = var_type[0].toUpper();
    if (var_value.isNull()) {
        return _FRD_ERROR_UNDEFINED_VARIABLE_;
    }

    if (var_type == "Layer") {
        // add another
        qDebug() << "Add layer here";
        auto arr = main["Layers"].toArray();
        arr.append(var_value);
        main["Layers"] = arr;
        return _FRD_NO_ERROR_;
    }
    else {
        if (main.keys().contains(var_type)) {
            main[var_type] = var_value;
            return _FRD_NO_ERROR_;
        }
        else return _FRD_ERROR_WRONG_PARAPETER_TYPE_;
    }
}

void FRD_Json::addError(FRD_error_type error_type, const QString& msg, int row, int col, int length) {
    QJsonObject obj;
    obj.insert("Type", error_type);
    obj.insert("Message", msg);
    obj.insert("Row", row);
    obj.insert("Col", col);
    obj.insert("Length", length);
    QJsonArray arr = main["Errors"].toArray();
    arr.append(obj);
    main["Errors"] = arr;
    qDebug() << "===Error===" << msg << row << col;
}

QString FRD_Json::toJson(QJsonDocument::JsonFormat format) const {
    QJsonDocument doc(main);
    return doc.toJson(format);
}

QString FRD_Json::varsToJson(QJsonDocument::JsonFormat format) const {
    QJsonDocument doc(vars);
    return doc.toJson(format);
}
