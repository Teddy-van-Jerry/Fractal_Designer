#include "FRD_Json.h"

FRD_Error::FRD_Error() : FRD_Error(_FRD_ERROR_OTHER_, 0, 0, 0, "") {}


FRD_Error::FRD_Error(int type, int row_, int col_, int length_, const QString& msg_)
    : error_type(FRD_error_type(type)),
      row(row_),
      col(col_),
      length(length_),
      msg(msg_) { }

FRD_Json::FRD_Json() {
    main.insert("Version", "6.0.6");
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
        else if (QVector<QString>({"Formula", "R", "G", "B", "H", "S", "V", "A", "X", "Y", "CentreX", "CentreY", "Width", "Angle", "Rate", "Distance", "Min", "Max", "IterationLimit"}).contains(member))
            return "formula";
        else if (QVector<QString>({"Route"}).contains(member))
            return "route";
        else if (QVector<QString>({"VideoDir", "ImageDir", "VideoFormat", "VideoName", "ImagePrefix", "Name", "Type"}).contains(member))
            return "string";
        else if (QVector<QString>({"Fps", "Time", "PreviewRotation", "PreviewTime"}).contains(member))
            return "number";
        else if (QVector<QString>({"Ts", "CentreXs", "CentreYs", "Widths", "Angles", "Rates", "ImageSize", "PreviewSize", "PreviewImageSize", "PreviewCentre"}).contains(member))
            return "array";
        else if (QVector<QString>({"InverseYAxis"}).contains(member))
            return "bool";
        else {
            QVector<QString> candidates;
            for (auto key : vars.keys()) {
                QStringList key_names = key.split('.', Qt::SkipEmptyParts);
                if (!key_names.isEmpty()) {
                    if (key_names.last() == names.last()) candidates.push_back(key);
                }
            }
            if (candidates.isEmpty()) return "string";
            else {
                // TODO in the future
                return vars[candidates[0]].toObject()["TYPE"].toString();
            }
        }
    }
}

QJsonValue FRD_Json::getValue(const QString& block, const QString& base_name, const QString& var_name) const {
    QStringList base_names = base_name.split('.', Qt::SkipEmptyParts);
    QStringList var_names = var_name.split('.', Qt::SkipEmptyParts);
    return getValue(block, base_names, var_names);
}

QJsonValue FRD_Json::getValue(const QString& block, const QStringList& base_names_, const QStringList& var_names_) const {
    QStringList base_names = base_names_;
    QStringList var_names  = var_names_;
    if (var_names_.size() == 0) return QJsonValue::Null;
    qDebug() << "=========>>>>>> getValue " << base_names_ << var_names_;
    QString block_ = block;
    QString base_names_str;
    for (const auto& name : base_names_) base_names_str += (name + ".");
    while(!base_names.isEmpty() && !contains(block_, base_names_str + var_names[0])) {
        // if the beginning of names is not the same,
        // they must be in the different block
        if (vars.contains(block_ + var_names[0])) {
            QString new_var_name = var_names.last();
            var_names.pop_back();
            return getValue(block_, var_names, {new_var_name});
        }
        do { block_.chop(1); } while(!block_.isEmpty() && block_.last(1) != ".");
        if (block_.isEmpty()) return QJsonValue::Null;
    }
    QJsonValue value;
    QStringList names = base_names + var_names;
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

FRD_error_type FRD_Json::setValue(const QString& block, const QString& base_name, const QString& var_name, const QString& type, QJsonValue value) {
    // If it is in place, set its value
    // if (setExistantValue(block, base_name, var_name, type, value) == _FRD_NO_ERROR_) return _FRD_NO_ERROR_;

    // Otherwise, create it
    QStringList names = (base_name + var_name).split('.', Qt::SkipEmptyParts);
    if (names.empty()) return _FRD_ERROR_UNDEFINED_VARIABLE_; // change into empty variable name
    QString key = block + names[0];

    qDebug() << "setValue reaches here with key" << key;

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
//    for (int i = names.size() - 1; i > 1; i--) {
//        if (refs[i].isObject()) {
//            auto obj = refs[i - 1].toObject();
//            obj[names[i]] = refs[i];
//            refs[i - 1] = obj;
//        }
//    }
//    if (names.size() > 1) {
//        QJsonObject obj = vars[names[0]].toObject();
//        obj["OBJECT"] = refs[1];
//        vars[names[0]] = obj;
//    }

    return _FRD_NO_ERROR_;
}

FRD_error_type FRD_Json::setExistantValue(const QString& block, const QString& base_name, const QString& var_name, const QString& type, QJsonValue value) {
    QStringList base_names = base_name.split('.', Qt::SkipEmptyParts);
    QStringList var_names  = var_name.split('.', Qt::SkipEmptyParts);
    return setExistantValue(block, base_names, var_names, type, value);
}

FRD_error_type FRD_Json::setExistantValue(const QString& block, const QStringList& base_names_, const QStringList& var_names_, const QString& type, QJsonValue value) {
    qDebug() << vars;
    QStringList base_names = base_names_;
    QStringList var_names  = var_names_;
    if (var_names.isEmpty()) return _FRD_ERROR_UNDEFINED_VARIABLE_; // change into empty variable name

    QString block_ = block;
    QString base_names_str;
    for (const auto& name : base_names_) base_names_str += (name + ".");
    while(!base_names.isEmpty() && !contains(block_, base_names_str + var_names[0])) {
        // if the beginning of names is not the same,
        // they must be in the different block
        if (vars.contains(block_ + var_names[0])) {
            QString new_var_name = var_names.last();
            var_names.pop_back();
            return setExistantValue(block_, var_names, {new_var_name}, type, value);
        }
        do { block_.chop(1); } while(!block_.isEmpty() && block_.last(1) != ".");
        if (block_.isEmpty()) return _FRD_ERROR_UNDEFINED_VARIABLE_;
    }

    QVector<QJsonValueRef> refs;
    QStringList names = base_names + var_names;
    if (names.size() == 1) {
        auto obj = vars[block_ + names[0]].toObject();
        obj["OBJECT"] = value;
        vars[block_ + names[0]] = value;
        return _FRD_NO_ERROR_;
    }

    refs.push_back(vars[block_ + names[0]].toObject()["OBJECT"]);
    for (int i = 1; i < names.size(); i++) {
        if (refs.last().toObject().contains(names[i])) {
            refs.push_back(refs.last().toObject()[names[i]]);
        }
        else {
            return _FRD_ERROR_UNDEFINED_VARIABLE_;
        }
    }
    refs.last() = value;
    qDebug() << vars;
    return _FRD_NO_ERROR_;
}

FRD_error_type FRD_Json::useValue(const QString& block, const QString& name) {
    qDebug() << "Entrance of useValue";
    if (!contains(block, name)) return _FRD_ERROR_UNDEFINED_VARIABLE_;
    auto var_value = getValue(block, {}, name);
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

void FRD_Json::clear() {
    main = QJsonObject();
    main.insert("Version", "6.0.6");
    main.insert("Time", QDateTime::currentDateTimeUtc().toString("yyyy/MM/dd hh:mm UTC"));
    main.insert("Layers", QJsonValue::Array);
    main.insert("Config", QJsonValue::Object);
    main.insert("Output", QJsonValue::Object);
    main.insert("Errors", QJsonValue::Array);
    vars = QJsonObject();
}

QString FRD_Json::text() const {
    return frd_text;
}

QVector<FRD_Error> FRD_Json::errors() const {
    QJsonArray arr = main["Errors"].toArray();
    QVector<FRD_Error> err_list;
    for (const auto& err : arr) {
        err_list.push_back(FRD_Error(err.toObject()["Type"].toInt(),
                                     err.toObject()["Row"].toInt(),
                                     err.toObject()["Col"].toInt(),
                                     err.toObject()["Length"].toInt(),
                                     err.toObject()["Message"].toString()));
    }
    return err_list;
}

void FRD_Json::updateText(QString text) {
    frd_text = text;
}

QString FRD_Json::layerFormula(int index) const {
    QJsonValue value = main["Layers"][index]["Template"]["Formula"];
    if (value.isString()) return value.toString();
    else return "0";
}

QString FRD_Json::layerColor(int index, QString tag) const {
    QStringList tags = tag.split('.', Qt::SkipEmptyParts);
    if (tags.size() != 2) return "0";
    QJsonValue value = main["Layers"][index]["Template"][tags[0]][tags[1]];
    return value.toString("0");
}

double FRD_Json::PreviewSize(QString tag) const {
    int index = (tag.toUpper() == "Y") ? 1 : 0;
    QJsonValue value = main["Config"]["PreviewSize"][index];
    return value.toDouble(0);
}

double FRD_Json::PreviewCentre(QString tag) const {
    int index = (tag.toUpper() == "Y") ? 1 : 0;
    QJsonValue value = main["Config"]["PreviewCentre"][index];
    return value.toDouble(0);
}

int FRD_Json::PreviewImageSize(QString tag) const {
    int index = (tag.toUpper() == "Y") ? 1 : 0;
    QJsonValue value = main["Config"]["PreviewImageSize"][index];
    return value.toInt(0);
}

double FRD_Json::PreviewRotation() const {
    QJsonValue value = main["Config"]["PreviewRotation"];
    return value.toDouble(0);
}

double FRD_Json::PreviewTime() const {
    QJsonValue value = main["Config"]["PreviewTime"];
    if (value.toDouble() < 0) return 0;
    if (value.toDouble() > 1) return 1;
    return value.toDouble(0);
}

bool FRD_Json::inverseYAsis() const {
    QJsonValue value = main["Config"]["InverseYAxis"];
    return value.toBool(false);
}

QString FRD_Json::templateMin(int index) const {
    QJsonValue value = main["Layers"][index]["Template"]["Min"];
    return value.toString("0");
}

QString FRD_Json::templateMax(int index) const {
    QJsonValue value = main["Layers"][index]["Template"]["Max"];
    return value.toString("10");
}

QString FRD_Json::iterationLimit(int index) const {
    QJsonValue value = main["Layers"][index]["Template"]["IterationLimit"];
    return value.toString("50");
}
