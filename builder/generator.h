#ifndef _GENERATOR_H
#define _GENERATOR_H

#include "astree.h"

/* This file dumps every function (and its derivs) into stdout assebmler file.
 * It also results each section of desired asm
 */

/// Adds a constant to storage
/// @return index of const
int addConstant(double val);

/// Recursively generates assembly code for AST function
void dumpNodeAsm(Node* node);

/// Generates assembly for each function within specified in SPEC_FILE ranges
void dumpTreeAsm(double rangeStart, double rangeEnd, Node** functions);

#endif
