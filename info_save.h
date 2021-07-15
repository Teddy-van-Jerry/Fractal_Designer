#ifndef INFO_SAVE_H
#define INFO_SAVE_H
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDebug>
#include <complex>
#include "route_info.h"
#include "preview_setting.h"

#define _ROUTE_SIZE_ 48

#ifndef For_All_Colour
#define For_All_Colour(it__, jt__) \
    for(int it__ = 0; it__ != 4; it__++) { \
        for(int jt__ = 0; jt__ != 29; jt__++) {
#define End_All_Colour }}
#endif

struct info
{
    info(char name_1_, char name_2_, qint32 length__) : length(length__)
    {
        name.resize(2);
        name[0] = name_1_;
        name[1] = name_2_;
    }
    QByteArray name;
    qint32 length;
};

struct Preview_Info
{
    int width = 800;
    int height = 600;
    double xWidth = 4;
    double yHeight = 3;
    double angle = 0;
    double centreX = 0;
    double centreY = 0;
    bool autoRefresh = false;
};

class Info_Save
{
    info Template {'T', 'E', 1};
    info Template_2 {'T', '2', 40}; // 16*2+8=40
    info Template_4 {'T', '4', 456}; // 16*28+8=456
    info ImageValue {'I', 'V', 20}; // 8+8+4+1=21
    info Colour1 {'C', '1', 0};
    info Colour2 {'C', '2', 0};
    info Route {'R', 'O', 0};
    info ImageOther {'I', 'O', 0};
    info VideoInfo {'V', 'I', 0};
    info CustomFormula {'C', 'F', 0};
    info Preview {'P', 'R', 49};

public:
    char template_ = 0;
    QString CustomFormula_;
    double min_class_v = 0, max_class_v = 0;
    qint32 max_loop_t = 50;
    bool y_inverse = false;
    QList<Route_Info> Route_;
    qint16 image_size_x = 1920, image_size_y = 1080;
    char frame_rate_index = 1;
    QString total_time_str = "0100";
    QString Colour1_f, Colour2_f;
    QString img_path;
    QString img_prefix;
    QString video_path;
    QString video_name;
    QList<QString> music_list;
    Preview_Info ps;

    ////////// Template 2 Only //////////
    std::complex<double> Julia_c1, Julia_c2;
    double Julia_c_rate = 0;
    /////////////////////////////////////

    ////////// Template 4 Only //////////
    std::complex<double> Newton_a_1 = 1;
    std::complex<double> Newton_a_2 = 1;
    std::complex<double> Newton_xn_1[10] {0};
    std::complex<double> Newton_xn_2[10] {0};
    std::complex<double> Newton_sin_1 = 0;
    std::complex<double> Newton_sin_2 = 0;
    std::complex<double> Newton_cos_1 = 0;
    std::complex<double> Newton_cos_2 = 0;
    std::complex<double> Newton_ex_1 = 0;
    std::complex<double> Newton_ex_2 = 0;
    double Newton_c_rate = 0;
    /////////////////////////////////////

    void setRouteInfo(const QList<Route_Info>& route_list)
    {
        Route_ = route_list;
        Route.length = route_list.size() * _ROUTE_SIZE_;
    }

    void setImageOtherInfo(const QString& img_path_, const QString& img_prefix_)
    {
        img_path   = img_path_;
        img_prefix = img_prefix_;
        ImageOther.length = 33 + sizeof(total_time_str) + sizeof(img_path) + sizeof(img_prefix);
    }

    void setVideoInfo(const QString& v_path, const QString& v_name, const QList<QString>& m_list)
    {
        video_path = v_path;
        video_name = v_name;
        music_list = m_list;
        VideoInfo.length = 2 + sizeof(video_path) + sizeof(video_name) + sizeof(music_list);
    }

    void setColourInfo(const QString& colour, bool conv)
    {
        if (conv)
        {
            Colour1.length = sizeof(colour);
            Colour1_f = colour;
        }
        else
        {
            Colour2.length = sizeof(colour);
            Colour2_f = colour;
        }
    }

    void setCustomFormulaInfo(const QString& str)
    {
        CustomFormula.length = sizeof(str);
        CustomFormula_ = str;
    }

    void print(QString path, uint8_t FRD_Version[4]);
};

#endif // INFO_SAVE_H
