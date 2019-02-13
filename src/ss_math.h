/* This is a work of the U.S. Government and is not subject to copyright
 * protection in the United States. Foreign copyrights may apply.
 * See copyright.txt for more information.
 */

#ifndef SS_MATH_H
#define SS_MATH_H

#include <math.h>


static double SS_PI = 3.1415926536;
static double SS_E  = 2.7182818285;

static double SS_SQRT_2PI = sqrt(2.0 * SS_PI);

float normal_dist (int num, int place)
{
    float interval = 2.0/num;
    float nrm = 0.0;
    float start = -1.0;
    float x = 0.0;
    if (num%2 == 0)
    {
        start += interval * .5;
    }
    x = start + interval * place;
    nrm = pow(SS_E, -(x * x / 4.0)) / SS_SQRT_2PI;
    return nrm;
}

#endif // SS_MATH_H
