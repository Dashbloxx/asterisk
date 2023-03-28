#pragma once

/*
 *  We don't create a `fabs` function, because we may want to pass an `int`, or `double` to it, and we can't have different versions of
 *  the same function with the same name. Anyways, this function returns the absolute value of the number that is passed as the argument.
 */
#define ABS(x) ((x) < 0 ? -(x) : (x))

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