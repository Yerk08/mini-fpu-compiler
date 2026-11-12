#ifndef _DERIVS_H
#define _DERIVS_H

#include "astree.h"

/* This file provides single function for calculating derives of some other func
 * via its AST tree.
 */

/// Generates AST for deriv of a function represented by its root node
Node* buildDerive(Node* root);

#endif
