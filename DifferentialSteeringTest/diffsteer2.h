/*
 * diffsteer2.h
 *
 *  Created on: Mar 30, 2009
 *      Author: dankrusi
 */

#ifndef DIFFSTEER2_H_
#define DIFFSTEER2_H_

#include <math.h>
#include <stdio.h>
#include <assert.h>


#define TYPE 1



#if TYPE == 1
typedef float real;
#define FORMAT "%.8e"
#endif

#if TYPE == 2
typedef double real;
#define FORMAT "%.16le"
#endif

#if TYPE == 3
typedef long double real;
#define FORMAT "%.20Le"
#endif



template <class real, int>
class FrenetT
{

};




typedef struct
{
    real coord [6];
}
diffeq;



real p = 0.3;
real q = 1;



double k (double t)
{
return t * sin (t);
    return 0;
}



diffeq ode (real x, diffeq y)
{
    diffeq result;

    double kk = k (x);

    result.coord [0] = y.coord [2];
    result.coord [1] = y.coord [3];
    result.coord [2] = y.coord [4] * kk;
    result.coord [3] = y.coord [5] * kk;
    result.coord [4] = - y.coord [2] * kk;
    result.coord [5] = - y.coord [3] * kk;

    return result;
}



diffeq add (diffeq v, diffeq w)
{
    diffeq u;
    int i;

    for (i = 0; i < 6; ++i)
        u.coord [i] = v.coord [i] + w.coord [i];

    return u;
}



diffeq sub (diffeq v, diffeq w)
{
    diffeq u;
    int i;

    for (i = 0; i < 6; ++i)
        u.coord [i] = v.coord [i] - w.coord [i];

    return u;
}



diffeq mul (real v, diffeq w)
{
    diffeq u;
    int i;

    for (i = 0; i < 6; ++i)
        u.coord [i] = v * w.coord [i];

    return u;
}



diffeq div (diffeq v, real w)
{
    return mul (1 / w, v);
}



diffeq Euler (real x0, diffeq y0, real h)
{
    return add (y0, mul (h, ode (x0, y0)));
}



diffeq RungeKutta (real x0, diffeq y0, real h)
{
    diffeq k1 = mul (h, ode (x0, y0));
    diffeq k2 = mul (h, ode (x0 + h / 2, add (y0, div (k1, 2))));
    diffeq k3 = mul (h, ode (x0 + h / 2, add (y0, div (k2, 2))));
    diffeq k4 = mul (h, ode (x0 + h, add (y0, k3)));
    return add (y0, div (add (add (k1, k4), mul (2, add (k2, k3))), 6));
}






void Solve
(
    diffeq (*method) (real, diffeq, real),
    real x0, diffeq y0,
    real h, real xmax
)
{

    for (;;)
    {
        //PlotPoint (x0, y0);

        if (x0 >= xmax)
            break;

        y0 = method (x0, y0, h);
        x0 += h;
    }

}

#endif /* DIFFSTEER2_H_ */
