#ifndef _CALCULATION_H
#define _CALCULATION_H

/* This file contains root() and integral() functions declaration and provides
 * the main calculation base for solve() function
 */

/// Short alias for double(func*)(double)
typedef double afunc(double);

/// Number of iterations performed by the root function
extern int iterCount;

/// Function for calculate roots of intersection between
/// @param f -- first function of intersection
/// @param g -- second function of intersection
/// @param a -- left boundary for search
/// @param b -- right boundary for search
/// @param df -- deriv. of first func
/// @param dg -- deriv. of second func
/// @param eps1 -- calculated precision
double root(afunc* f, afunc* g, double a, double b, double eps1, afunc* df, afunc* dg);

/// Function for calculate integral of function
/// @param f -- function for integration
/// @param a -- left boundary for integral
/// @param b -- right boundary for integral
/// @param eps2 -- calculated precision
double integral(afunc* f, double a, double b, double eps2);

#endif
