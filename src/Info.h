#ifndef INFO_H
#define INFO_H

#include <QString>
#include <QVector>
#include <QFile>
#include <QFileInfo>
#include "FRD_Json.h"

class Info {
private:
    QVector<FRD_Json> all_;  /**< historic info */
    int i = -1;              /**< current info index */
    FRD_Json editor_;        /**< current info of editor */

public:
    // return the current version
    FRD_Json& curr();
    FRD_Json curr() const;
    FRD_Json& editor();
    FRD_Json editor() const;

    bool print(const QString& path) const;
};

#endif // INFO_H
