#ifndef _SOLUTION_H
#define _SOLUTION_H

/* This file provides solve() function as main calculation, which utilizes root()
 * and integral(). Also it uses A and B global constants (which are defined in asm file)
 * and printFlags, which are debug printed in solve function
 */

/// for debug print in static double getRoot()
/// 0x1 - print root, 0x2 - print iterations, 0x4 - print integrals
extern char printFlags;

/// Boundary of function root search
extern double A, B;

/// Short alias for double(func*)(double)
typedef double afunc(double);

/// Find roots and print them if printFlags were set
double getRoot(afunc* f, afunc* g, double a, double b, double eps, afunc* df, afunc* dg);

/// Main function for resolve the task
/// @param n -- size of func list
/// @param f -- function list with size @param n
/// @param df -- deriv func list with size @param n
double solve(int n, afunc* f[n], afunc* df[n]);

#endif
