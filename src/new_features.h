#ifndef NEW_FEATURES_H
#define NEW_FEATURES_H

#include <QDialog>
#include <QBrush>
#include <QColor>

namespace Ui {
class New_Features;
}

class New_Features : public QDialog
{
    Q_OBJECT

public:
    explicit New_Features(QWidget *parent = nullptr);
    ~New_Features();

private:
    Ui::New_Features *ui;
};

#endif // NEW_FEATURES_H
