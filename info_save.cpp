#include "info_save.h"
#include "mainwindow.h"

void _complex_out(QDataStream& out, std::complex<double> c)
{
    out << c.real() << c.imag();
}

void Info_Save::print(QString path, uint8_t FRD_Version[4])
{
    /// Preparation for Print Only Info ///

    ///////////////////////////////////////

    QFile FRD_S(path);
    FRD_S.open(QIODevice::WriteOnly);
    QDataStream out(&FRD_S);
    uint8_t md5[16] = {0};
    char magic[8] = {'F', 'R', 'D', -17, 'T', 'v', 'J', 42 }; // magic number : "FRD\xEFTvJ*"
    for(int i = 0; i != 16; i++) // 16
    {
        out << md5[i];
    }
    for(int i = 0; i != 8; i++) // 24
    {
        out << magic[i];
    }
    for(int i = 0; i != 4; i++) // 28
    {
        out << FRD_Version[i];
    }

    // Template
    out << Template.name[0] << Template.name[1] << Template.length;
    out << template_;

    // Template2
    if (template_ == 2)
    {
        out << Template_2.name[0] << Template_2.name[1] << Template_2.length;
        out << Julia_c1.real() << Julia_c1.imag()
            << Julia_c2.real() << Julia_c2.imag()
            << Julia_c_rate;
    }

    // Template4
    else if (template_ == 4)
    {
        out << Template_4.name[0] << Template_4.name[1] << Template_4.length;
        _complex_out(out, Newton_a_1);
        _complex_out(out, Newton_a_2);
        for(int i = 0; i != 10; i++) _complex_out(out, Newton_xn_1[i]);
        for(int i = 0; i != 10; i++) _complex_out(out, Newton_xn_2[i]);
        _complex_out(out, Newton_sin_1);
        _complex_out(out, Newton_sin_2);
        _complex_out(out, Newton_cos_1);
        _complex_out(out, Newton_cos_2);
        _complex_out(out, Newton_ex_1);
        _complex_out(out, Newton_ex_2);
        out << Newton_c_rate;
    }

    // Customized Formula (Template 5)
    out << CustomFormula.name[0] << CustomFormula.name[1] << CustomFormula.length;
    out << CustomFormula_;

    // Image Value
    out << ImageValue.name[0] << ImageValue.name[1] << ImageValue.length;
    out << min_class_v << max_class_v << max_loop_t << y_inverse;

    out << Colour1.name[0] << Colour1.name[1] << Colour1.length << Colour1_f;

    out << Colour2.name[0] << Colour2.name[1] << Colour2.length << Colour2_f;

    // Route
    out << Route.name[0] << Route.name[1] << Route.length;
    // qDebug() << Route.length;
    for(const auto& c : Route_)
    {
        c.print(out);
    }

    // Image Other
    out << ImageOther.name[0] << ImageOther.name[1] << ImageOther.length;
    out << image_size_x << image_size_y << frame_rate_index << total_time_str << img_path << img_prefix;

    // Video Info
    out << VideoInfo.name[0] << VideoInfo.name[1] << VideoInfo.length;
    out << video_path << video_name << static_cast<quint16>(music_list.size());
    for(const auto& c : music_list)
    {
        out << c;
    }

    // Preview
    out << Preview.name[0] << Preview.name[1] << Preview.length;
    out << ps.width << ps.height << ps.xWidth << ps.yHeight << ps.angle << ps.centreX << ps.centreY << ps.autoRefresh;

    FRD_S.close();
}
