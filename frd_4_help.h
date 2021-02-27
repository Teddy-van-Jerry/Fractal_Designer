#ifndef FRD_4_HELP_H
#define FRD_4_HELP_H

#include <QMainWindow>

namespace Ui {
class FRD_4_Help;
}

class FRD_4_Help : public QMainWindow
{
    Q_OBJECT

public:
    explicit FRD_4_Help(QWidget *parent = nullptr);
    ~FRD_4_Help();

private slots:
    void on_actionBack_to_main_window_triggered();

private:
    Ui::FRD_4_Help *ui;
};

#endif // FRD_4_HELP_H
