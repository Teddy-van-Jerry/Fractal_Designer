#include "Info.h"

FRD_Json& Info::curr() {
    // TODO in the future
    return editor_;
}

FRD_Json Info::curr() const {
    // TODO in the future
    return editor_;
}

FRD_Json& Info::editor() {
    return editor_;
}

FRD_Json Info::editor() const {
    return editor_;
}

bool Info::print(const QString& path) const {
    if (path.isEmpty()) return false;
    QFile file(path);
    file.open(QIODeviceBase::WriteOnly);
    QTextStream out(&file);
    QFileInfo file_info(file);
    if (file_info.suffix() == "frd") out << curr().text();
    else out << curr().toJson();
    file.close();
    return true;
}
