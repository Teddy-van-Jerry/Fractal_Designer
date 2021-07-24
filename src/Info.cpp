#include "Info.h"

//QVariant frd_class_base_::operator[](int i) const {
//    return member_vars[i];
//}

//QVariant& frd_class_base_::operator[](int i) {
//    return member_vars[i];
//}

int frd_class_base_::index(const QString& name) const {
    int i = 0;
    for (; i != member_names.size(); i++) {
        if (member_names[i] == name) break;
    }
    return i;
}

bool frd_class_base_::contains(const QString& name) const {
    return this->index(name) != member_names.size();
}

QStringList frd_class_base_::members() const {
    return member_names;
}

int FRD_var_list_::index(const QString &name) const {
    for (int i = 0; i != this->size(); i++) {
        if (this->at(i).varName == name) return i;
    }
    return this->size(); // does not contain
}

bool FRD_var_list_::contains(const QString& name) const {
    return this->size() != this->index(name);
}

FRD_var_ FRD_var_list_::operator[](const QString& name) const {
    return this->at(this->index(name));
}

FRD_var_& FRD_var_list_::operator[](const QString& name) {
    int index = this->index(name);
    return *(this->begin() + index);
}

void FRD_var_list_::addVar(const FRD_class_& cls, const QString& name, const QVariant& value, int row_, int col_) {
    FRD_var_ var { cls, name, value, row_, col_ };
}

QVariant range_::operator[](const QString& name) const {
    if (name == "From") return From;
    if (name == "To")   return To;
    return QVariant();
}

QVariant& range_::operator[](const QString& name) {
    if (name == "From") return From;
    if (name == "To")   return To;
}
