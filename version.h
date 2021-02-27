#ifndef VERSION_H
#define VERSION_H

#include <QDialog>

namespace Ui {
class Version;
}

class Version : public QDialog
{
    Q_OBJECT

public:
    explicit Version(QWidget *parent = nullptr);
    ~Version();

private slots:
    void on_pushButton_Help_clicked();

    void on_pushButton_New_clicked();

    void on_pushButton_Update_clicked();

private:
    Ui::Version *ui;
};

#endif // VERSION_H
