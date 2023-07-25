#include "math.h"

double sin(double x) {
    double result = 0.0;
    double term = x;
    int n = 1;

    while(ABS(term) >= 1e-8) {
        result += term;
        term *= (-1.0) * x * x / (2 * n) / (2 * n + 1);
        n++;
    }

    return result;
}

double cos(double x) {
    double result = 1.0;
    double term = 1.0;
    int n = 1;

    while (ABS(term) >= 1e-8) {
        term *= (-1.0) * x * x / (2 * n - 1) / (2 * n);
        result += term;
        n++;
    }

    return result;
}

double tan(double x) {
    double result = 0.0;
    double term = x;
    double numerator = x;
    double denominator = 1.0;
    int n = 1;

    while (ABS(term) >= 1e-8) {
        numerator *= x * x;
        denominator *= 2 * n - 1;
        term = numerator / denominator;
        result += (n % 2 == 0 ? -1 : 1) * term;
        n++;
    }

    return result;
}

double asin(double x) {
    if (x < -1.0 || x > 1.0) {
        return NAN;
    } else if (x == 1.0) {
        return M_PI_2;
    } else if (x == -1.0) {
        return -M_PI_2;
    } else {
        double result = x;
        double term = x;
        double numerator = x;
        double denominator = 1.0;
        int n = 1;

        while (ABS(term) >= 1e-8) {
            numerator *= x * x * (2 * n - 1) * (2 * n - 1);
            denominator *= 2 * n;
            term = numerator / denominator;
            result += term;
            n++;
        }

        return result;
    }
}

double acos(double x) {
    if (x < -1.0 || x > 1.0) {
        return NAN;
    } else if (x == 1.0) {
        return 0.0;
    } else if (x == -1.0) {
        return M_PI;
    } else {
        double result = M_PI_2;
        double term = x;
        double numerator = x;
        double denominator = 1.0;
        int n = 1;

        while (ABS(term) >= 1e-8) {
            numerator *= x * x * (2 * n - 1) * (2 * n - 1);
            denominator *= 2 * n;
            term = numerator / denominator;
            result -= term;
            n++;
        }

        return result;
    }
}

double atan(double x) {
    double result = 0.0;
    double term = x;
    double numerator = x;
    double denominator = 1.0;
    int sign = 1;
    int n = 1;

    while (ABS(term) >= 1e-8) {
        result += sign * term;
        numerator *= x * x;
        denominator = 2 * n + 1;
        term = numerator / denominator;
        sign *= -1;
        n++;
    }

    return result;
}