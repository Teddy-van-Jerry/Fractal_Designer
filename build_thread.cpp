#include "build_thread.h"
#include "mainwindow.h"
#include <QDebug>

Build_Thread::Build_Thread(QObject* parent) : QObject(parent)
{

}

void Build_Thread::drawImage(double x, double y, double x_width, double y_height, int X, int Y, double rotate_angle, double t,
                             QString img_format, QString img_path, QString img_title, QString work_name)
{
    qDebug() << "Got it here";
//    MainWindow* p = (MainWindow*)parent();

    y = -y;

    // Colour_Data[0][3][28][1] = Colour_Data[1][3][28][1] = 255; // set Opaque

    if(!Version_Higher_Than_4) // low version
    {
        QFile Colour_saved_1(pro_path + "/Colour_Set_1.txt");
        if(Colour_saved_1.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in1(&Colour_saved_1);
            for(int j = 0; j != 4; j++)
            {
                in1 >> Colour_Data[0][j][0][0] >> Colour_Data[0][j][0][1];
                in1 >> Colour_Data[0][j][1][0] >> Colour_Data[0][j][1][1];
                in1 >> Colour_Data[0][j][2][0] >> Colour_Data[0][j][2][1];
                in1 >> Colour_Data[0][j][3][0] >> Colour_Data[0][j][3][1];
                in1 >> Colour_Data[0][j][4][0] >> Colour_Data[0][j][4][1];
                in1 >> Colour_Data[0][j][5][0] >> Colour_Data[0][j][5][1];
                in1 >> Colour_Data[0][j][6][0] >> Colour_Data[0][j][6][1];
                in1 >> Colour_Data[0][j][7][0] >> Colour_Data[0][j][7][1];
                in1 >> Colour_Data[0][j][8][0] >> Colour_Data[0][j][8][1];
                in1 >> Colour_Data[0][j][9][0] >> Colour_Data[0][j][9][1];
                in1 >> Colour_Data[0][j][10][0] >> Colour_Data[0][j][10][1];
                in1 >> Colour_Data[0][j][11][0] >> Colour_Data[0][j][11][1];
                in1 >> Colour_Data[0][j][12][0] >> Colour_Data[0][j][12][1];
                in1 >> Colour_Data[0][j][13][0] >> Colour_Data[0][j][13][1];
                in1 >> Colour_Data[0][j][14][0] >> Colour_Data[0][j][14][1];
                in1 >> Colour_Data[0][j][15][0] >> Colour_Data[0][j][15][1];
                in1 >> Colour_Data[0][j][16][0] >> Colour_Data[0][j][16][1];
                in1 >> Colour_Data[0][j][17][0] >> Colour_Data[0][j][17][1];
                in1 >> Colour_Data[0][j][18][0] >> Colour_Data[0][j][18][1];
                in1 >> Colour_Data[0][j][19][0] >> Colour_Data[0][j][19][1];
                in1 >> Colour_Data[0][j][20][0] >> Colour_Data[0][j][20][1];
                in1 >> Colour_Data[0][j][21][0] >> Colour_Data[0][j][21][1];
                in1 >> Colour_Data[0][j][22][0] >> Colour_Data[0][j][22][1];
                in1 >> Colour_Data[0][j][23][0] >> Colour_Data[0][j][23][1];
                in1 >> Colour_Data[0][j][24][0] >> Colour_Data[0][j][24][1];
                in1 >> Colour_Data[0][j][25][0] >> Colour_Data[0][j][25][1];
                in1 >> Colour_Data[0][j][26][0] >> Colour_Data[0][j][26][1];
                in1 >> Colour_Data[0][j][27][0] >> Colour_Data[0][j][27][1];
                in1 >> Colour_Data[0][j][28][0] >> Colour_Data[0][j][28][1];
            }
            Colour_saved_1.close();
        }

        QFile Colour_saved_2(pro_path + "/Colour_Set_2.txt");
        if(Colour_saved_2.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in2(&Colour_saved_2);
            for(int j = 0; j != 4; j++)
            {
                in2 >> Colour_Data[1][j][0][0] >> Colour_Data[1][j][0][1];
                in2 >> Colour_Data[1][j][1][0] >> Colour_Data[1][j][1][1];
                in2 >> Colour_Data[1][j][2][0] >> Colour_Data[1][j][2][1];
                in2 >> Colour_Data[1][j][3][0] >> Colour_Data[1][j][3][1];
                in2 >> Colour_Data[1][j][4][0] >> Colour_Data[1][j][4][1];
                in2 >> Colour_Data[1][j][5][0] >> Colour_Data[1][j][5][1];
                in2 >> Colour_Data[1][j][6][0] >> Colour_Data[1][j][6][1];
                in2 >> Colour_Data[1][j][7][0] >> Colour_Data[1][j][7][1];
                in2 >> Colour_Data[1][j][8][0] >> Colour_Data[1][j][8][1];
                in2 >> Colour_Data[1][j][9][0] >> Colour_Data[1][j][9][1];
                in2 >> Colour_Data[1][j][10][0] >> Colour_Data[1][j][10][1];
                in2 >> Colour_Data[1][j][11][0] >> Colour_Data[1][j][11][1];
                in2 >> Colour_Data[1][j][12][0] >> Colour_Data[1][j][12][1];
                in2 >> Colour_Data[1][j][13][0] >> Colour_Data[1][j][13][1];
                in2 >> Colour_Data[1][j][14][0] >> Colour_Data[1][j][14][1];
                in2 >> Colour_Data[1][j][15][0] >> Colour_Data[1][j][15][1];
                in2 >> Colour_Data[1][j][16][0] >> Colour_Data[1][j][16][1];
                in2 >> Colour_Data[1][j][17][0] >> Colour_Data[1][j][17][1];
                in2 >> Colour_Data[1][j][18][0] >> Colour_Data[1][j][18][1];
                in2 >> Colour_Data[1][j][19][0] >> Colour_Data[1][j][19][1];
                in2 >> Colour_Data[1][j][20][0] >> Colour_Data[1][j][20][1];
                in2 >> Colour_Data[1][j][21][0] >> Colour_Data[1][j][21][1];
                in2 >> Colour_Data[1][j][22][0] >> Colour_Data[1][j][22][1];
                in2 >> Colour_Data[1][j][23][0] >> Colour_Data[1][j][23][1];
                in2 >> Colour_Data[1][j][24][0] >> Colour_Data[1][j][24][1];
                in2 >> Colour_Data[1][j][25][0] >> Colour_Data[1][j][25][1];
                in2 >> Colour_Data[1][j][26][0] >> Colour_Data[1][j][26][1];
                in2 >> Colour_Data[1][j][27][0] >> Colour_Data[1][j][27][1];
                in2 >> Colour_Data[1][j][28][0] >> Colour_Data[1][j][28][1];
            }
            Colour_saved_2.close();
        }
    }

    qDebug() << "The working thread: " << QThread::currentThreadId();
    double progress = 0;
    int progress_now = 0;

    if(!Version_Higher_Than_4)
    {
        QFile Template_(pro_path + "/Template_Choice.txt");
        if(Template_.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in3(&Template_);
            in3 >> template_;
            Template_.close();
        }

        QFile Define_Value_(pro_path + "/Define_Value.txt");
        if(Define_Value_.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in4(&Define_Value_);
            in4 >> min_class_v >> max_class_v >> max_loop_t;
            Define_Value_.close();
        }
    }

//    qDebug() << QThread::currentThreadId();
    QImage image_build(X, Y, QImage::Format_ARGB32);
    for(int i = X - 1; i >= 0; i--)
    {
        progress = 100 * static_cast<double>(X - i) / X;
        if(static_cast<int>(progress) > progress_now)
        {
            if(work_name == "Preview") emit progressInform_preview(progress);
            if(work_name == "Route") emit progressInform_route(progress);
            progress_now = static_cast<int>(progress);
        }
        for(int j = Y - 1; j >= 0; j--)
        {
            double dx = x_width * (static_cast<double>(i) / X - 0.5);
            double dy = y_height * (static_cast<double>(j) / Y - 0.5);
            double mod = sqrt(dx * dx + dy * dy);
            double theta = atan2(dy, dx) + rotate_angle / 180 * Pi;
            Complex this_point(x + mod * cos(theta),
                               y + mod * sin(theta));
            Complex z0(this_point);
            bool test = true;
            int k = 0;
            for (k = 0; k < max_loop_t; k++)
            {
                if ((double)this_point.modulus() > max_class_v)
                {
                    test = false;
                    break;
                }
                else if ((double)this_point.modulus() < min_class_v)
                {
                    test = true;
                    break;
                }
                else
                {
                    if(template_ == 1) this_point = (this_point ^ 2.0) + z0;
                }
            }

            if(test)
            {
                double RGBA1[4];
                for(int m = 0; m != 4; m++)
                {
                    RGBA1[m] = (Colour_Data[0][m][0][0]  * t + Colour_Data[0][m][0][1])  * k +
                               (Colour_Data[0][m][1][0]  * t + Colour_Data[0][m][1][1])  * this_point.getReal() +
                               (Colour_Data[0][m][2][0]  * t + Colour_Data[0][m][2][1])  * this_point.getImaginary() +
                               (Colour_Data[0][m][3][0]  * t + Colour_Data[0][m][3][1])  * (double)this_point.modulus() +
                               (Colour_Data[0][m][4][0]  * t + Colour_Data[0][m][4][1])  * (double)this_point.modulus() / min_class_v +
                               (Colour_Data[0][m][5][0]  * t + Colour_Data[0][m][5][1])  * this_point.argz() +
                               (Colour_Data[0][m][6][0]  * t + Colour_Data[0][m][6][1])  * sin(this_point.argz()) +
                               (Colour_Data[0][m][7][0]  * t + Colour_Data[0][m][7][1])  * cos(this_point.argz()) +
                               (Colour_Data[0][m][8][0]  * t + Colour_Data[0][m][8][1])  * z0.getReal() +
                               (Colour_Data[0][m][9][0]  * t + Colour_Data[0][m][9][1])  * z0.getImaginary() +
                               (Colour_Data[0][m][10][0] * t + Colour_Data[0][m][10][1]) * (double)z0.modulus() +
                               (Colour_Data[0][m][11][0] * t + Colour_Data[0][m][11][1]) * (double)z0.modulus() / min_class_v +
                               (Colour_Data[0][m][12][0] * t + Colour_Data[0][m][12][1]) * z0.argz() +
                               (Colour_Data[0][m][13][0] * t + Colour_Data[0][m][13][1]) * sin(z0.argz()) +
                               (Colour_Data[0][m][14][0] * t + Colour_Data[0][m][14][1]) * (double)(this_point - z0).modulus() +
                               (Colour_Data[0][m][15][0] * t + Colour_Data[0][m][15][1]) * exp(k) +
                               (Colour_Data[0][m][16][0] * t + Colour_Data[0][m][16][1]) * exp(log(10) * k) +
                               (Colour_Data[0][m][17][0] * t + Colour_Data[0][m][17][1]) * log(1 + (double)this_point.modulus()) +
                               (Colour_Data[0][m][18][0] * t + Colour_Data[0][m][18][1]) * log(1 + (double)z0.modulus()) +
                               (Colour_Data[0][m][19][0] * t + Colour_Data[0][m][19][1]) * log(1 + (double)(this_point - z0).modulus()) +
                               (Colour_Data[0][m][20][0] * t + Colour_Data[0][m][20][1]) * exp((double)this_point.modulus()) +
                               (Colour_Data[0][m][21][0] * t + Colour_Data[0][m][21][1]) * exp((double)z0.modulus()) +
                               (Colour_Data[0][m][22][0] * t + Colour_Data[0][m][22][1]) * exp((double)(this_point - z0).modulus()) +
                               (Colour_Data[0][m][23][0] * t + Colour_Data[0][m][23][1]) * exp(log(10) * (double)this_point.modulus()) +
                               (Colour_Data[0][m][24][0] * t + Colour_Data[0][m][24][1]) * exp(log(10) * (double)z0.modulus()) +
                               (Colour_Data[0][m][25][0] * t + Colour_Data[0][m][25][1]) * exp(log(10) * (double)(this_point - z0).modulus()) +
                               (Colour_Data[0][m][26][0] * t + Colour_Data[0][m][26][1]) * exp((double)this_point.modulus() / min_class_v) +
                               (Colour_Data[0][m][27][0] * t + Colour_Data[0][m][27][1]) * exp((double)z0.modulus() / min_class_v) +
                               (Colour_Data[0][m][28][0] * t + Colour_Data[0][m][28][1]);
                    if(RGBA1[m] < 0) RGBA1[m] = 0;
                    if(RGBA1[m] > 255) RGBA1[m] = 255;
                }
                image_build.setPixel(i, j, qRgba(RGBA1[0], RGBA1[1], RGBA1[2], RGBA1[3]));
                //qDebug() << RGBA1[0] << " " << RGBA1[1] << " " << RGBA1[2] << " "<<RGBA1[3];
            }
            else
            {
                double RGBA2[4];
                for(int m = 0; m != 4; m++)
                {
                    RGBA2[m] = (Colour_Data[1][m][0][0]  * t + Colour_Data[1][m][0][1])  * k +
                               (Colour_Data[1][m][1][0]  * t + Colour_Data[1][m][1][1])  * this_point.getReal() +
                               (Colour_Data[1][m][2][0]  * t + Colour_Data[1][m][2][1])  * this_point.getImaginary() +
                               (Colour_Data[1][m][3][0]  * t + Colour_Data[1][m][3][1])  * (double)this_point.modulus() +
                               (Colour_Data[1][m][4][0]  * t + Colour_Data[1][m][4][1])  * (double)this_point.modulus() / max_class_v +
                               (Colour_Data[1][m][5][0]  * t + Colour_Data[1][m][5][1])  * this_point.argz() +
                               (Colour_Data[1][m][6][0]  * t + Colour_Data[1][m][6][1])  * sin(this_point.argz()) +
                               (Colour_Data[1][m][7][0]  * t + Colour_Data[1][m][7][1])  * cos(this_point.argz()) +
                               (Colour_Data[1][m][8][0]  * t + Colour_Data[1][m][8][1])  * z0.getReal() +
                               (Colour_Data[1][m][9][0]  * t + Colour_Data[1][m][9][1])  * z0.getImaginary() +
                               (Colour_Data[1][m][10][0] * t + Colour_Data[1][m][10][1]) * (double)z0.modulus() +
                               (Colour_Data[1][m][11][0] * t + Colour_Data[1][m][11][1]) * (double)z0.modulus() / max_class_v +
                               (Colour_Data[1][m][12][0] * t + Colour_Data[1][m][12][1]) * z0.argz() +
                               (Colour_Data[1][m][13][0] * t + Colour_Data[1][m][13][1]) * sin(z0.argz()) +
                               (Colour_Data[1][m][14][0] * t + Colour_Data[1][m][14][1]) * (double)(this_point - z0).modulus() +
                               (Colour_Data[1][m][15][0] * t + Colour_Data[1][m][15][1]) * exp(k) +
                               (Colour_Data[1][m][16][0] * t + Colour_Data[1][m][16][1]) * exp(log(10) * k) +
                               (Colour_Data[1][m][17][0] * t + Colour_Data[1][m][17][1]) * log(1 + (double)this_point.modulus()) +
                               (Colour_Data[1][m][18][0] * t + Colour_Data[1][m][18][1]) * log(1 + (double)z0.modulus()) +
                               (Colour_Data[1][m][19][0] * t + Colour_Data[1][m][19][1]) * log(1 + (double)(this_point - z0).modulus()) +
                               (Colour_Data[1][m][20][0] * t + Colour_Data[1][m][20][1]) * exp((double)this_point.modulus()) +
                               (Colour_Data[1][m][21][0] * t + Colour_Data[1][m][21][1]) * exp((double)z0.modulus()) +
                               (Colour_Data[1][m][22][0] * t + Colour_Data[1][m][22][1]) * exp((double)(this_point - z0).modulus()) +
                               (Colour_Data[1][m][23][0] * t + Colour_Data[1][m][23][1]) * exp(log(10) * (double)this_point.modulus()) +
                               (Colour_Data[1][m][24][0] * t + Colour_Data[1][m][24][1]) * exp(log(10) * (double)z0.modulus()) +
                               (Colour_Data[1][m][25][0] * t + Colour_Data[1][m][25][1]) * exp(log(10) * (double)(this_point - z0).modulus()) +
                               (Colour_Data[1][m][26][0] * t + Colour_Data[1][m][26][1]) * exp((double)this_point.modulus() / max_class_v) +
                               (Colour_Data[1][m][27][0] * t + Colour_Data[1][m][27][1]) * exp((double)z0.modulus() / max_class_v) +
                               (Colour_Data[1][m][28][0] * t + Colour_Data[1][m][28][1]);
                    if(RGBA2[m] < 0) RGBA2[m] = 0;
                    if(RGBA2[m] > 255) RGBA2[m] = 255;
                }
                image_build.setPixel(i, j, qRgba(RGBA2[0], RGBA2[1], RGBA2[2], RGBA2[3]));
                //if(i == 20 && j == 20) qDebug() << RGBA2[0] << " " << RGBA2[1] << " " << RGBA2[2] << " "<<RGBA2[3];
                //image_build.setPixel(20, 20, qRgba(10, 10, 10, 10));
            }
        }
    }
    qDebug() << img_path;
    image_build.save(img_path + "/" + img_title + "." + img_format);
    image_build.load(img_path + "/" + img_title + "." + img_format);

    if(work_name == "Preview") emit updateImage_preview(image_build);
    if(work_name == "Route") emit updateImage_route(image_build);
    if(work_name == "Create_Image") emit one_ok();
    if(work_name == "Create_Image_Last") emit finished();
}

void Build_Thread::setPath(QString str)
{
    pro_path = str;
}

void Build_Thread::setData(double C1[4][29][2], double C2[4][29][2], int temp, double min, double max, int lpt)
{
    For_All_Colour(i, j)
        Colour_Data[0][i][j][0] = C1[i][j][0];
        Colour_Data[0][i][j][1] = C1[i][j][1];
        Colour_Data[1][i][j][0] = C2[i][j][0];
        Colour_Data[1][i][j][1] = C2[i][j][1];
    End_All_Colour

    template_   = temp;
    max_class_v = max;
    min_class_v = min;
    max_loop_t  = lpt;
}

void Build_Thread::setVersion(bool v)
{
    Version_Higher_Than_4 = v;
}
