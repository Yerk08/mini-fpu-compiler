#ifndef _TESTING_H
#define _TESTING_H

/// Short alias for double(func*)(double)
typedef double afunc(double);

/// Testing root() function via list of funcs and their derivs
/// @param str -- F1:F2:A:B:E:R parameter list
int testRoot(char* str, int n, afunc* f[n], afunc* df[n]);

/// Testing integral() function via list of funcs
/// @param str -- F:A:B:E:R parameter list
int testIntegral(char* str, int n, afunc* f[n]);

/// Functions for test main module
double testF1(double);
double testF2(double);
double testF3(double);
double testDF1(double);
double testDF2(double);
double testDF3(double);

/// Testing functions arrays definition
extern afunc* testFuncs[3];
extern afunc* testDerivs[3];

#endif
