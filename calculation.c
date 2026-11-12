#include "calculation.h"

#include <math.h>
#include <stdbool.h>

int iterCount;

double root(afunc* f, afunc* g, double a, double b, double eps1, afunc* df, afunc* dg) {
    double mid = (a + b) / 2.0;
    double dfMid = df(mid) - dg(mid);

    // evaluation of dd(f - g) via chord
    double chordMid = ((f(a) - g(a)) + (f(b) - g(b))) / 2.0;
    double ddfMid = chordMid - (f(mid) - g(mid));
    // check if F'(x) * F''(x) > 0
    bool sameSign = (dfMid * ddfMid > 0.0);

    // if sameSign is true, d = b (approach from right)
    // else d = a (approach from left)
    double xCurr = sameSign ? b : a;
    double xNext;

    iterCount = 0;  // reset global iteration counter

    for (int i = 0; i < 300; ++i) {  // always finite number of steps
        ++iterCount;
        double fx = f(xCurr) - g(xCurr);
        double dfx = df(xCurr) - dg(xCurr);

        if (fabs(dfx) < 1e-15) {
            return xCurr;  // avoid division by zero
        }

        xNext = xCurr - fx / dfx;
        double fc = f(xNext) - g(xNext);

        if (!sameSign) {
            // left (d = a): compare signs F(c) & F(c + eps)
            double diff = f(xNext + eps1) - g(xNext + eps1);
            if (fc * diff <= 0.0) {
                return xNext;  // root is within [c, c + eps]
            }
        } else {
            // right (d = b): compare signs F(c - eps) & F(c)
            double diff = f(xNext - eps1) - g(xNext - eps1);
            if (fc * diff <= 0.0) {
                return xNext;  // root is within [c - eps, c]
            }
        }

        xCurr = xNext;
    }

    return xCurr;
}

double integral(afunc* f, double a, double b, double eps2) {
    int n = 10;
    double h = (b - a) / n;
    double midSum = 0.0;
    // remember values: F_0, F_n
    double edgeSum = (f(a) + f(b)) / 2.0;

    // appending middle: F_1, F_2, ..., F_(n-1)
    for (int i = 1; i < n; ++i) {
        midSum += f(a + i * h);
    }

    double iOld = h * (edgeSum + midSum);

    for (int step = 0; step < 300; ++step) {  // always finite number of steps
        n *= 2;
        h = (b - a) / n;

        double oddSum = 0.0;
        for (int i = 1; i < n; i += 2) {
            oddSum += f(a + i * h);
        }

        midSum += oddSum;  // appending odd values: F_1, F_3, ...
                           // (because F_2, F_4, ... were calculated previously)
        double iNew = h * (edgeSum + midSum);
        if (fabs(iNew - iOld) / 3.0 < eps2) {  // Runge's rule
            return iNew;
        }

        iOld = iNew;
    }

    return iOld;
}
