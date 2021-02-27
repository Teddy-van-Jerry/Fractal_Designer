#ifndef OPEN_FILE_H
#define OPEN_FILE_H

#include <QDialog>

namespace Ui {
class Open_File;
}

class Open_File : public QDialog
{
    Q_OBJECT

public:
    explicit Open_File(QWidget *parent = nullptr);
    ~Open_File();

private slots:
    void on_button_cancel_clicked();

    void on_button_confirm_open_clicked();

    void on_open_path_edit_textEdited(const QString &arg1);

    void on_open_browse_clicked();

signals:

    void template_choice(int n);

    void define_value(double, double, int);

    void image_size(int, int);

    void route_info(QVector<double>);

    void image_path(QString);

    void image_prefix(QString);

    void frame_rate(int);

    void total_time(QString);

    void music_added(QVector<QString>);

private:
    Ui::Open_File *ui;
};

#endif // OPEN_FILE_H
