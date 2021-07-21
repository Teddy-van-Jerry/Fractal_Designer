#ifndef ROUTE_INFO_H
#define ROUTE_INFO_H
#include <QDataStream>

struct Route_Info
{
    double t = 0;
    double x = 0;
    double y = 0;
    double angle = 0;
    double width = 0;
    double v_rate = 0;

    Route_Info read(QDataStream& in)
    {
        in >> t >> x >> y >> angle >> width >> v_rate;
        return *this;
    }

    void print(QDataStream& out) const
    {
        out << t << x << y << angle << width << v_rate;
    }

    bool operator==(const Route_Info& i) const
    {
        return (t == i.t && x == i.x && y == i.y && angle == i.angle && width == i.width && v_rate == i.v_rate);
    }

    bool operator!=(const Route_Info& i) const
    {
        return !(*this == i);
    }
};

#endif // ROUTE_INFO_H
