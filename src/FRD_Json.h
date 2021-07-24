#ifndef FRD_JSON_H
#define FRD_JSON_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include <QDateTime>

class FRD_Json {
public:
    FRD_Json();

    QJsonValue operator[](const QString& key) const;

    QJsonValueRef operator[](const QString& key);

    // set the value
    // if name does not exist, check in the wider block
    bool setGlobalValue(const QString& name, const QString& type, QJsonValue value);

    // set the value
    // if name does not exist, create it
    bool setValue(const QString& name, const QString& type, QJsonValue value);

    bool setValue(const QStringList& names, const QString& type, QJsonValue value);

    // put the value into main
    bool useValue(const QString& varName);

private:
    QJsonDocument doc;
    QJsonObject main;  // main object of doc
    QJsonObject vars;  // key by their names
};

#endif // FRD_JSON_H
