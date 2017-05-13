#include "conversions.h"

float stringToFloat(QString s_flt)
{
    bool okay = false;
    float flt = s_flt.toFloat(&okay);
    while (!okay)
    {
        s_flt.chop(1);
        flt = s_flt.toFloat(&okay);
    }
    return flt;
}

int stringToInt(QString s_int)
{
    bool okay = false;
    int n = s_int.toFloat(&okay);
    while (!okay)
    {
        s_int.chop(1);
        n = s_int.toFloat(&okay);
    }
    return n;
}

