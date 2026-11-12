#ifndef _OPTIMIZATIONS_H
#define _OPTIMIZATIONS_H

#include "astree.h"

/* This file provides some optimizations AST -> AST, which are update the root from
 * the entire tree, and for utilize them need to write `optimize(root);`
 */

/// Perform constant propagation for function of @param root and @return new root
Node* optimizeConstants(Node* root);

/// Optimizes FPU stack usage, does not changes the @param root and @return height of the tree
int optimizeFpuStack(Node* root);  // ignore return value

/// Perform reduction for function of @param root and @return new root
Node* optimizeReduction(Node* root);

/// Perform square hints for optimizations of @param root node
void optimizeHintsSquares(Node* root);

#endif
