#ifndef _BUILDING_H
#define _BUILDING_H

#include "astree.h"

/* This is main file of asm builder. It provides parsing function main()
 * and printHelpFunc() for view help menu.
 */

/// Flags for debug printing
extern char flagsTree;  // 0x1 - debug AST,
                        // 0x2 - optimize FPU stack and constants,
                        // 0x4 - functions reduction and optimization

/// Auxiliary function for print help menu
void printHelpFunc(void);

#endif
