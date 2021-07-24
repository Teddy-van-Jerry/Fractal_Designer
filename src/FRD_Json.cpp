#include "FRD_Json.h"

FRD_Json::FRD_Json() {
    main.insert("Version", "6.0.2");
    main.insert("Time", QDateTime::currentDateTimeUtc().toString("yyyy/MM/dd hh:mm UTC"));
    main.insert("Layers", QJsonValue::Array);
    main.insert("Config", QJsonValue::Object);
    main.insert("Output", QJsonValue::Object);
}

QJsonValue FRD_Json::operator[](const QString& key) const {
    return main[key];
}

bool FRD_Json::setGlobalValue(const QString& name, const QString& type, QJsonValue value) {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    if (names.size() == 0) return false;
    while (!setValue(names, type, value)) {
        if (names.size() < 2) return false; // can not find
        *(names.end() - 2) = names.last();
        names.pop_back();
    }
    return true;
}

bool FRD_Json::setValue(const QString& name, const QString& type, QJsonValue value) {
    QStringList names = name.split('.', Qt::SkipEmptyParts);
    return setValue(names, type, value);
}

bool FRD_Json::setValue(const QStringList& names, const QString& type, QJsonValue value) {
    if (names.isEmpty()) return false;

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
        return true;
    }
    else {
        // Can not convert
        return false;
    }
}

bool FRD_Json::useValue(const QString& varName) {
    if (!vars.contains(varName)) return false;
    auto var = vars[varName].toObject();
    QString type_name = var["TYPE"].toString();
    if (type_name == "Layer") {
        // add another
        main["Layers"].toArray().append(var["OBJECT"].toObject());
        return true;
    }
    else {
        if (main.contains(type_name)) {
            main[type_name].toObject() = var["OBJECT"].toObject();
            return true;
        }
        else return false;
    }
}
