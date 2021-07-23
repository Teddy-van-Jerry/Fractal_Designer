#include "Info.h"

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
