#ifndef ROUTE_TOOL_H
#define ROUTE_TOOL_H

#include <QDialog>
#include <QMouseEvent>
#include <QPainter>

#ifndef Pi
#define Pi 3.14159265358979323846
#endif

namespace Ui {
class Route_Tool;
}

class Route_Tool : public QDialog
{
    Q_OBJECT

    friend class Paint_Event;

public:
    explicit Route_Tool(QWidget *parent = nullptr);
    ~Route_Tool();

    double width_to_size = 1;

private:
    Ui::Route_Tool *ui;

    double centreX;
    double centreY;
    double img_size;
    double t;

    QString pro_path;

    QImage image;

    bool press_move = false;

    double preX = 0, preY = 0, preCentreX = 0, preCentreY = 0;

    int dx = 0, dy = 0;

    QObject* p = parent();

    void paintEvent(QPaintEvent* event);

signals:
    void build_signal(double x, double y, double x_width, double y_height, int X, int Y, double rotate_angle, double t,
                      QString img_format, QString img_path, QString img_title, QString work_name);
private slots:
    void on_pushButton_view_clicked();

    void on_comboBox_axisColour_currentIndexChanged(int index);

    void on_Slider_time_valueChanged(int value);

    void on_doubleSpinBox_t_editingFinished();

    void on_actionBack_to_main_window_triggered();

    void on_actionView_triggered();

    void on_pushButton_Save_clicked();

    void on_actionSave_Point_triggered();

    void on_action0_5_triggered();

    void on_action0_8_triggered();

    void on_action1_triggered();

    void on_action1_2_triggered();

    void on_action1_5_triggered();

    void on_action2_0_triggered();

protected:
    void mouseMoveEvent(QMouseEvent* e);

    void mouseDoubleClickEvent(QMouseEvent* e);

    void mouseReleaseEvent(QMouseEvent* e);

    void wheelEvent(QWheelEvent* e);

public slots:
    void getImage(QImage);

    void updateProgressBar(double);

    void setPath(QString);
};

#endif // ROUTE_TOOL_H
