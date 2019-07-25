
#include "comassist.h"

float normalizeDegreeN180_180( float degree )
{
    while( degree > 180 )
    { degree -= 360; }
    while( degree <= -180 )
    { degree += 360; }

    return degree;
}
