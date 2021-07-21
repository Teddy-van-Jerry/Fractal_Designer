#ifndef NEW_FILE_H
#define NEW_FILE_H

#include <QDialog>

namespace Ui {
class New_File;
}

class New_File : public QDialog
{
    Q_OBJECT

public:
    explicit New_File(QWidget *parent = nullptr);
    ~New_File();

private slots:

    void on_button_cancel_clicked();

    void on_button_confirm_new_clicked();

    void on_new_browse_clicked();

private:
    Ui::New_File *ui;

    int type = 1;

};

#endif // NEW_FILE_H
