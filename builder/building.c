#include "building.h"
#include "astree.h"
#include "debugging.h"
#include "derivs.h"
#include "generator.h"
#include "optimizations.h"
#include "parser.h"

#include <stdio.h>
#include <string.h>

char flagsTree = 0;

/// This function display hard-coded description of command-line args.
void printHelpFunc(void) {
    printf("Usage: ./building [OPTION]...\n\n");
    printf("Generate assembler file with derivs to output for input functions.\n");
    printf("  -h, --help                 display this help and exit\n");
    printf("  -t, --tree                 print abstract syntax tree for functions\n");
    printf("  -o, --optimize             optimizes FPU stack and constants\n");
    printf("  -r, --reduction            reduction functions and optimize\n");
    printf("\nExample:\n");
    printf("  $ SPEC_FILE=test.txt ./building\n");
    printf("  global A, B, f1, f2, f3, df1, df2, df3\n  ...\n");
}

/// Main parser function
int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printHelpFunc();
            return 0;
        }

        else if (strcmp(argv[i], "--tree") == 0 || strcmp(argv[i], "-t") == 0) {
            flagsTree |= 0x1;  // enabling debug print of AST
        }

        else if (strcmp(argv[i], "--optimize") == 0 || strcmp(argv[i], "-o") == 0) {
            flagsTree |= 0x2;  // enables automatic reversions for reduce FPU stack
            // and enables constant propagation of binary and unary operations
        }

        else if (strcmp(argv[i], "--reduction") == 0 || strcmp(argv[i], "-r") == 0) {
            flagsTree |= 0x4;  // reduce unnecessary calculations for binary operations
            // and enables square optimizations "fmul s0, s0"
        }

        else {
            printf("No flag found\n");
            return 1;
        }
    }

    double rangeStart = 0.0;  // initial A from SPEC_FILE
    double rangeEnd = 0.0;    // initial B from SPEC_FILE

    Node* functions[6];  // resulting functions

    if (loadSpecification(&rangeStart, &rangeEnd, functions) != 0) {
        fprintf(stderr, "Pipeline aborted due to compilation errors.\n");
        return 1;
    }

    for (int i = 3; i < 6; ++i) {
        functions[i] = buildDerive(functions[i - 3]);
    }

    for (int i = 0; i < 6; ++i) {
        if (flagsTree & 0x4)
            functions[i] = optimizeReduction(functions[i]);

        if (flagsTree & 0x2) {
            functions[i] = optimizeConstants(functions[i]);
            optimizeFpuStack(functions[i]);
        }

        if (flagsTree & 0x4)
            optimizeHintsSquares(functions[i]);
    }

    // printing AST for debugging Nodes
    if (flagsTree & 0x1) {
        for (int i = 0; i < 6; ++i) {
            fprintf(stderr, "-- Debug tree for func %cf%d:\n\t", i < 3 ? ' ' : 'd',
                    i % 3 + 1);
            debugPrintTree(stderr, functions[i]);
            fprintf(stderr, "\n");
        }
    }

    // dumping result transformations into stdout
    dumpTreeAsm(rangeStart, rangeEnd, functions);

    // clean up allocated space of ASTs
    for (int i = 0; i < 6; ++i) {
        if (functions[i] != NULL) {
            freeTree(functions[i]);
        }
    }

    return 0;
}
