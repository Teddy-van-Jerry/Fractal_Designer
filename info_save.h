#ifndef INFO_SAVE_H
#define INFO_SAVE_H
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QDebug>
#include "route_info.h"

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

class Info_Save
{
    info Template {'T', 'E', 1};
    info ImageValue {'I', 'V', 20}; // 8+8+4=20
    info Colour1 {'C', '1', 1856}; // 29*2*4*8=1856
    info Colour2 {'C', '2', 1856}; // 29*2*4*8=1856
    info Route {'R', 'O', 0};
    info ImageOther {'I', 'O', 0};
    info VideoInfo {'V', 'I', 0};
    info Config {'C', 'O', 1};

public:
    char template_ = 0;
    double min_class_v = 0, max_class_v = 0;
    qint32 max_loop_t = 50;
    double Colour_Data_1[4][29][2] = {0};
    double Colour_Data_2[4][29][2] = {0};
    QList<Route_Info> Route_;
    qint16 image_size_x = 1920, image_size_y = 1080;
    char frame_rate_index = 1;
    QString total_time_str = "0100";
    QString img_path;
    QString img_prefix;
    QString video_path;
    QString video_name;
    QList<QString> music_list;
    //////// Only Valid in Print ////////
    char config1 = 0;
    /////////////////////////////////////

    void setRouteInfo(QList<Route_Info> route_list)
    {
        Route_ = route_list;
        Route.length = route_list.size() * _ROUTE_SIZE_;
    }

    void setImageOtherInfo(QString img_path_, QString img_prefix_)
    {
        img_path   = img_path_;
        img_prefix = img_prefix_;
        ImageOther.length = 33 + sizeof(total_time_str) + sizeof(img_path) + sizeof(img_prefix);
    }

    void setVideoInfo(QString v_path, QString v_name, QList<QString> m_list)
    {
        video_path = v_path;
        video_name = v_name;
        music_list = m_list;
        VideoInfo.length = 2 + sizeof(video_path) + sizeof(video_name) + sizeof(music_list);
    }

    void print(QString path, uint8_t FRD_Version[4]);
};

#endif // INFO_SAVE_H
