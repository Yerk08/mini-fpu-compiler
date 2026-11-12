#include "solution.h"
#include "calculation.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

/// macros for fast swapping (and sorting) two functions in solve()
#define SWAP_IF_LESS(i, j)             \
    if (f[p[i]](mid) < f[p[j]](mid)) { \
        int t = p[i];                  \
        p[i] = p[j];                   \
        p[j] = t;                      \
    }

/// auxiliary sort evaluated roots abscissas (for take ordering of x1, x2, x3)
static void sortBubble(int n, double arr[n]) {
    for (int i = 0; i < n; i++) {
        bool sorted = true;

        for (int j = 0; j < n - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                sorted = false;

                double temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }

        if (sorted)
            return;
    }
}

/// get root with debug print
double getRoot(afunc* f, afunc* g, double a, double b, double eps, afunc* df, afunc* dg) {
    double r = root(f, g, a, b, eps, df, dg);
    if (printFlags & 0x1)
        printf("-- root: %.4lf\n", r);
    if (printFlags & 0x2)
        printf("-- iterations: %d\n", iterCount);
    return r;
}

double solve(int n, afunc* f[n], afunc* df[n]) {
    double xRoot[3];            // evaluated roots abscissas x
    double epsRoot = 1e-5;      // eps1
    double epsIntegral = 1e-4;  // eps2
    double totalArea = 0.0;     // resulting area
    int rootIdx = 0;

    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            xRoot[rootIdx++] = getRoot(f[i], f[j], A, B, epsRoot, df[i], df[j]);

    sortBubble(rootIdx, xRoot);  // take ordering of x1, x2, x3

    for (int i = 0; i < rootIdx - 1; i++) {
        // now we are on the segment
        double left = xRoot[i];
        double right = xRoot[i + 1];
        double mid = (left + right) / 2.0;

        // sort function indices by height: p[0]=top, p[1]=mid, p[2]=bot
        int p[3] = {0, 1, 2};
        SWAP_IF_LESS(0, 1);
        SWAP_IF_LESS(1, 2);
        SWAP_IF_LESS(0, 1);
        int topId = p[0], midId = p[1], botId = p[2];

        // circ and corc Idx (circ means ^ function, when corc - v of the area)
        int circIdx = -1, corcIdx = -1;  // circ(^) and corc(v)

        // the bounded area by checking endpoints
        if (fabs(f[topId](left) - f[midId](left)) < 1e-3 ||
            fabs(f[topId](right) - f[midId](right)) < 1e-3) {
            circIdx = topId;  // area is between top and mid
            corcIdx = midId;
        } else if (fabs(f[midId](left) - f[botId](left)) < 1e-3 ||
                   fabs(f[midId](right) - f[botId](right)) < 1e-3) {
            circIdx = midId;  // area is between mid and bot
            corcIdx = botId;
        } else {
            continue;  // enclosed target region
        }

        afunc* top = f[circIdx];
        afunc* bot = f[corcIdx];

        if (printFlags & 0x4) {
            printf("-- functions: f%d - f%d\n", circIdx + 1, corcIdx + 1);
            printf("   range: %.4lf .. %.4lf\n", left, right);
            printf("   add: %.8lf\n", integral(top, left, right, epsIntegral));
            printf("   sub: %.8lf\n", integral(bot, left, right, epsIntegral));
        }

        // appending integral between top and bot
        totalArea += integral(top, left, right, epsIntegral) -
                     integral(bot, left, right, epsIntegral);
    }

    return totalArea;
}
