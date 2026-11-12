#include "testing.h"
#include "calculation.h"
#include "solution.h"

#include <math.h>
#include <stdio.h>

int testRoot(char* str, int n, afunc* f[n], afunc* df[n]) {
    int f1n, f2n;
    double a, b, eps1, result;
    int parsed = sscanf(str, "%d:%d:%lf:%lf:%lf:%lf", &f1n, &f2n, &a, &b, &eps1, &result);

    if (parsed != 6) {
        printf("Wrong parameters set\n");
        return 1;
    }

    // number of functions is hard-coded in integral.c file for stability
    if (f1n < 1 || f1n > n || f2n < 1 || f2n > n) {
        printf("Function numbers must be between 1 and %d\n", n);
        return 1;
    }

    // calc - evaluated result of testing root()
    double calc = getRoot(f[f1n - 1], f[f2n - 1], a, b, eps1, df[f1n - 1], df[f2n - 1]);

    double err = fabs(calc - result);
    double relErr = err / fabs(result);
    // printing in exactly this format as stated in Techinal Spec.
    printf("%.5lf %.5lf %.6lf\n", calc, err, relErr);
    return 0;
}

int testIntegral(char* str, int n, afunc* f[n]) {
    int fn;
    double a, b, eps2, result;
    int parsed = sscanf(str, "%d:%lf:%lf:%lf:%lf", &fn, &a, &b, &eps2, &result);

    if (parsed != 5) {
        printf("Wrong parameters set\n");
        return 1;
    }

    if (fn < 1 || fn > n) {
        printf("Function number must be between 1 and %d\n", n);
        return 1;
    }

    // calc - evaluated result of testing integral()
    double calc = integral(f[fn - 1], a, b, eps2);

    double err = fabs(calc - result);
    double relErr = err / fabs(result);
    // printing in exactly this format as stated in Techinal Spec.
    printf("%.5lf %.5lf %.6lf\n", calc, err, relErr);
    return 0;
}

afunc* testFuncs[3] = {testF1, testF2, testF3};
afunc* testDerivs[3] = {testDF1, testDF2, testDF3};

/// Functions for testing defined here:
double testF1(double var) {
    return 1 + 4 / (var * var + 1);
}
double testF2(double var) {
    return var * var * var;
}
double testF3(double var) {
    return pow(2, -var);
}

/// Functions derives definition:
double testDF1(double var) {
    return -8 * var / pow(var * var + 1, 2);
}
double testDF2(double var) {
    return 3 * var * var;
}
double testDF3(double var) {
    return -log(2) * pow(2, -var);
}
