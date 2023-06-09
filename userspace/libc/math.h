#pragma once

/*
 *  We don't create a `fabs` function, because we may want to pass an `int`, or `double` to it, and we can't have different versions of
 *  the same function with the same name. Anyways, this function returns the absolute value of the number that is passed as the argument.
 */
#define ABS(x) ((x) < 0 ? -(x) : (x))

/*
 *  NaN = Not a Number
 */
#define NAN (0.0 / 0.0)

/*
 *  The short value of Pi is also defined here, as it may be useful when calculating certain things.
 */
#define M_PI 3.14159265358979323846

/*
 *  This is the value of Pi, divided by two.
 */
#define M_PI_2 1.57079632679489661923

/* The sine function */
double sin(double x);
/* The cosine function */
double cos(double x);
/* The tangent function */
double tan(double x);
/* Inverse sine function */
double asin(double x);
/* Inverse cosine function */
double acos(double x);
/* Inverse tangent function */
double atan(double x);