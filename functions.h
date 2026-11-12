#ifndef _FUNCTIONS_H
#define _FUNCTIONS_H

/* This file intends to be the communication between function.asm
 * and other project C-functions. function.h is header only without an object file.
 */

/// Boundary declared here for asm definition
extern double A, B;

/// Main calculation functions of the task
double f1(double);
double f2(double);
double f3(double);

/// Functions' derives for Newton method roots finding
double df1(double);
double df2(double);
double df3(double);

#endif
