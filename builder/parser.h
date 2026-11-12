#ifndef _PARSER_H
#define _PARSER_H

#include <stdio.h>
#include "astree.h"

/* This file contains functions for parsing SPEC_FILE and building ASTree
 *
 */

/// Function only opens the file by env $(SPEC_FILE)
FILE* openSpecFile(void);

/// Parsing line via string, using the stack for reverse-polish notation
Node* parseLine(char* line);

/// Global function for parsing entire file via the parseLine()
int processFileLines(FILE* file, double* rangeStart, double* rangeEnd, Node** functions);

/// Combination of previous functions. @return 1 if error occurs and 0 if okay
int loadSpecification(double* rangeStart, double* rangeEnd, Node** functions);

#endif
