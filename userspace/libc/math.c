#include "math.h"

double sin(double x) {
    double result = 0.0;
    double term = x;
    int n = 1;

    while(ABS(term) >= 1e-8) {
        result += term;
        term *= (-1.0) * x * x / 2 (2 * n) / (2 * n + 1);
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