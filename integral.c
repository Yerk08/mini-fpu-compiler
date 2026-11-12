#include "integral.h"
#include "calculation.h"
#include "functions.h"
#include "solution.h"
#include "testing.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

char printFlags = 0x0;  // no flag set for debug print

/// This function display hard-coded description
void printHelpFunc(void) {
    printf("Usage: ./integral [OPTION]...\n\n");
    printf("Calculate the area of a bounded figure or test numerical functions.\n");
    printf("With no OPTION, prints the final calculated area.\n\n");
    printf("  -h, --help                 display this help and exit\n");
    printf("  -r, --root                 print abscissas of curve intersections\n");
    printf("  -i, --iterations           print iteration counts for root finding\n");
    printf("  -l, --integrals ARG        print integrals of functions between\n");
    printf("  -R, --test-root ARG        test root function with F1:F2:A:B:E:R\n");
    printf("  -I, --test-integral ARG    test integral function with F:A:B:E:R\n");
    printf("\nExample:\n");
    printf("  $ ./integral --test-root 1:2:0.5:5.0:0.0001:3.84776\n");
    printf("  1.34365 2.50411 0.650797\n");
}

/// Main parser function
int main(int argc, char* argv[]) {
    afunc* funcs[3] = {f1, f2, f3};
    afunc* derivs[3] = {df1, df2, df3};

    bool exitFlag = false;  // This flag accepts putting multiple test options

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printHelpFunc();
            return 0;
        }

        else if (strcmp(argv[i], "--root") == 0 || strcmp(argv[i], "-r") == 0) {
            printFlags |= 0x1;  // This flag in getRoot() function prints roots
        }

        else if (strcmp(argv[i], "--iterations") == 0 || strcmp(argv[i], "-i") == 0) {
            printFlags |= 0x2;  // Activates getRoot() iterations debug count print
        }

        else if (strcmp(argv[i], "--integrals") == 0 || strcmp(argv[i], "-l") == 0) {
            printFlags |= 0x4;  // Activates solve() integrals debug
        }

        else if (strcmp(argv[i], "--test-root") == 0 || strcmp(argv[i], "-R") == 0) {
            exitFlag = true;  // Exit after all checks
            if (i + 1 >= argc) {
                printHelpFunc();
                return 1;
            }

            if (testRoot(argv[i + 1], 3, funcs, derivs))
                return 1;
            ++i;  // skip next test-formatted string
        }

        else if (strcmp(argv[i], "--test-integral") == 0 || strcmp(argv[i], "-I") == 0) {
            exitFlag = true;  // Exit after all checks
            if (i + 1 >= argc) {
                printHelpFunc();
                return 1;
            }

            if (testIntegral(argv[i + 1], 3, funcs))
                return 1;
            ++i;  // skip next test-formatted string
        }

        else {
            printf("No flag found\n");
            return 1;
        }
    }
    if (exitFlag)
        return 0;

    double area = solve(3, funcs, derivs);
    printf("%.5lf\n", area);
}
