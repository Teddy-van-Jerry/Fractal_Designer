#include "route_info.h"

QDataStream& operator<<(QDataStream& out, const Route_Info& i)
{
    out << i.t << i.x << i.y << i.angle << i.width << i.v_rate;
    return out;
}
