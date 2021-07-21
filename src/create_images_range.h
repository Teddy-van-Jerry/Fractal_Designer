#ifndef CREATE_IMAGES_RANGE_H
#define CREATE_IMAGES_RANGE_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class Create_Images_Range;
}

class Create_Images_Range : public QDialog
{
    Q_OBJECT

public:
    explicit Create_Images_Range(QWidget *parent, int limit_number);
    ~Create_Images_Range();

private slots:
    void on_pushButton_go_clicked();

private:
    Ui::Create_Images_Range *ui;

    int limit = 0;
};

#endif // CREATE_IMAGES_RANGE_H
