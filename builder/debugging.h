#ifndef _DEBUGGING_H
#define _DEBUGGING_H

#include "astree.h"
#include "stdio.h"

/* This file accepts debugging AST via the debugPrintTree() function.
 * Mainly used in building.c file and generator.c for inline comments.
 */

/// Debug function for print AST tree of some @param node to @param out
void debugPrintTree(FILE* out, Node* node);

#endif
