////////////////////////////////////////////////////////////////////////
///                                                                  ///
///   file: vector2.h                                                ///
///                                                                  ///
////////////////////////////////////////////////////////////////////////



#ifndef VECTOR2_H_INCLUDED
#define VECTOR2_H_INCLUDED



#include "math-ext.h"

#include <cassert>
#include <iostream>
#include <iomanip>



template <class real>
class Vector2T
{
        //  data

 real Data [2];


    public:

        //  constructors

            Vector2T (void);
            Vector2T (real, real);


        //  conversions

            Vector2T (const Vector2T <float> &);
            Vector2T (const Vector2T <double> &);
            Vector2T (const Vector2T <long double> &);


        //  components

            real x (void) const;
            real y (void) const;

            real operator () (int) const;

            real & x (void);
            real & y (void);

            real & operator () (int);


        //  unary operators

            const Vector2T <real> & operator + (void) const;
            const Vector2T <real> operator - (void) const;


        //  binary operators

            Vector2T operator + (const Vector2T &) const;
            Vector2T operator - (const Vector2T &) const;
            Vector2T operator * (real) const;
            Vector2T operator / (real) const;


        //  assignment operators

            Vector2T & operator =  (const Vector2T &);
            Vector2T & operator += (const Vector2T &);
            Vector2T & operator -= (const Vector2T &);
            Vector2T & operator *= (real);
            Vector2T & operator /= (real);
};



////////////////////////////////////////////////////////////////////////
///   constructors                                                   ///
////////////////////////////////////////////////////////////////////////

template <class real>
Vector2T <real> :: Vector2T (void)
{
}



template <class real>
Vector2T <real> :: Vector2T (real xx, real yy)
{
    x() = xx;
    y() = yy;
}



////////////////////////////////////////////////////////////////////////
///   conversions                                                    ///
////////////////////////////////////////////////////////////////////////

template <class real>
Vector2T <real> :: Vector2T (const Vector2T <float> & v)
{
    x() = (real) v.x(); y() = (real) v.y();
}



template <class real>
Vector2T <real> :: Vector2T (const Vector2T <double> & v)
{
    x() = (real) v.x();
    y() = (real) v.y();
}



template <class real>
Vector2T <real> :: Vector2T (const Vector2T <long double> & v)
{
    x() = (real) v.x();
    y() = (real) v.y();
}



////////////////////////////////////////////////////////////////////////
///   components                                                     ///
////////////////////////////////////////////////////////////////////////

template <class real>
real Vector2T <real> :: x (void) const
{
    return Data[0];
}



template <class real>
real Vector2T <real> :: y (void) const
{
    return Data[1];
}


template <class real>
real Vector2T <real> :: operator () (int i) const
{
    assert (0 <= i && i < 2);
    return Data[i];
}



template <class real>
real & Vector2T <real> :: x (void)
{
    return Data[0];
}



template <class real>
real & Vector2T <real> :: y (void)
{
    return Data[1];
}



template <class real>
real & Vector2T <real> :: operator () (int i)
{
    assert (0 <= i && i < 2);
    return Data[i];
}



////////////////////////////////////////////////////////////////////////
///   unary operators                                                ///
////////////////////////////////////////////////////////////////////////

template <class real>
const Vector2T <real> & Vector2T <real> :: operator + (void) const
{
    return *this;
}



template <class real>
const Vector2T <real> Vector2T <real> :: operator - (void) const
{
    return Vector2T <real> (-x(), -y());
}



////////////////////////////////////////////////////////////////////////
///   binary operators                                               ///
////////////////////////////////////////////////////////////////////////

template <class real>
Vector2T <real> Vector2T <real> :: operator + (const Vector2T <real> & v) const
{
    return Vector2T <real> (x() + v.x(), y() + v.y());
}



template <class real>
Vector2T <real> Vector2T <real> :: operator - (const Vector2T <real> & v) const
{
    return Vector2T <real> (x() - v.x(), y() - v.y());
}



template <class real>
Vector2T <real> Vector2T <real> :: operator * (real s) const
{
    return Vector2T <real> (x() * s, y() * s);
}



template <class real>
Vector2T <real> Vector2T <real> :: operator / (real s) const
{
    return Vector2T <real> (x() / s, y() / s);
}



template <class real>
Vector2T <real> operator * (real s, const Vector2T <real> & v)
{
    return Vector2T <real> (v.x() * s, v.y() * s);
}



template <class real>
Vector2T <real> & Vector2T <real> :: operator = (const Vector2T <real> & v)
{
    x() = v.x(); y() = v.y(); return *this;
}



template <class real>
Vector2T <real> & Vector2T <real> :: operator += (const Vector2T <real> & v)
{
    x() += v.x(); y() += v.y(); return *this;
}



template <class real>
Vector2T <real> & Vector2T <real> :: operator -= (const Vector2T <real> & v)
{
    x() -= v.x(); y() -= v.y(); return *this;
}



template <class real>
Vector2T <real> & Vector2T <real> :: operator *= (real s)
{
    x() *= s; y() *= s; return *this;
}



template <class real>
Vector2T <real> & Vector2T <real> :: operator /= (real s)
{
    x() /= s; y() /= s; return *this;
}



////////////////////////////////////////////////////////////////////////
///   dot product and related functions                              ///
////////////////////////////////////////////////////////////////////////

template <class real>
real dot (const Vector2T <real> & v, const Vector2T <real> & w)
{
    return v.x() * w.x() + v.y() * w.y();
}



template <class real>
real dist (const Vector2T <real> & v, const Vector2T <real> & w)
{
    return norm (v - w);
}



template <class real>
real norm (const Vector2T <real> & v)
{
    return Sqrt (dot (v, v));
}



template <class real>
Vector2T <real> normalize (const Vector2T <real> & v)
{
    real s = norm (v);
    return s == 0 ? v : v / s;
}



////////////////////////////////////////////////////////////////////////
///   determinant                                                    ///
////////////////////////////////////////////////////////////////////////

template <class real>
real det (const Vector2T <real> & a, const Vector2T <real> & b)
{
    return a.x() * b.y() - a.y() * b.x();
}



////////////////////////////////////////////////////////////////////////
///   text input and output                                          ///
////////////////////////////////////////////////////////////////////////

template <class real>
std :: istream & operator >> (std :: istream & file, Vector2T <real> & v)
{
    return file
        >> v.x()
        >> v.y();
}



template <class real>
std :: ostream & operator << (std :: ostream & file, const Vector2T <real> & v)
{
    int w = file.width();

    return file
        << std :: setw (w) << v.x() << ' '
        << std :: setw (w) << v.y();
}



////////////////////////////////////////////////////////////////////////
///   typedefs                                                       ///
////////////////////////////////////////////////////////////////////////

typedef Vector2T <float> Vector2F;
typedef Vector2T <double> Vector2D;
typedef Vector2T <long double> Vector2L;



#endif
