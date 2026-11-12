#include "generator.h"
#include "astree.h"
#include "debugging.h"

#include <stdio.h>

/// Intermediate constant enumeration for further dump
static int constantCount = 0;
/// Intermediate constant list
static double dataConstants[1024];

/// Auxiliary inner structure for addConstant() evaluation
union BitDouble {
    double a;
    long long b;
};

int addConstant(double val) {
    for (int i = 0; i < constantCount; ++i) {
        // if const was found previously, don't waste memory for it
        if (((union BitDouble)dataConstants[i]).b == ((union BitDouble)val).b)
            return i;
    }
    dataConstants[constantCount] = val;
    return constantCount++;
}

void dumpNodeAsm(Node* node) {
    if (!node)
        return;

    dumpNodeAsm(node->left);
    if (!node->isDuplicated)
        dumpNodeAsm(node->right);

    switch (node->type) {
        case NODE_NUMBER: {
            int idx = addConstant(node->data.value);
            printf("    fld qword [const%d]   ; ", idx + 1);
            break;
        }
        case NODE_VARIABLE:
            printf("    fld qword [ebp + 8]  ; ");
            break;

        case NODE_BINARY:
            switch (node->data.op) {
                case OP_ADD:
                    printf("    faddp st1, st0       ; ");
                    break;
                case OP_SUB:
                    // if node was swapped via -s flag, then
                    // it needs to use reverse instruction
                    if (node->isSwapped) {
                        printf("    fsubrp               ; ");
                    } else {
                        printf("    fsubp                ; ");
                    }
                    break;
                case OP_MUL:
                    // if nodes left and right are equal, then
                    // it needs to use single fmul operation
                    if (node->isDuplicated) {
                        printf("    fmul st0, st0        ; ");
                    } else {
                        printf("    fmulp st1, st0       ; ");
                    }
                    break;
                case OP_DIV:
                    // when right and left nodes were reversed
                    // then it needs to swap it directly in asm
                    if (node->isSwapped) {
                        printf("    fdivrp               ; ");
                    } else {
                        printf("    fdivp                ; ");
                    }
                    break;
                default:
                    break;
            }
            break;

        case NODE_UNARY:
            switch (node->data.op) {
                case OP_SIN:
                    printf("    fsin                 ; ");
                    break;
                case OP_COS:
                    printf("    fcos                 ; ");
                    break;
                case OP_TAN:
                    // fptan pushes 1.0, then tan => pop 1.0
                    printf("    fptan\n");
                    printf("    fxch st1             ; ");
                    break;
                case OP_CTG:
                    // ctg(x) = cos(x)/sin(x) => using fptan and inverting
                    printf("    fptan\n");
                    printf("    fdivp                ; ");
                    break;
                default:
                    break;
            }
            break;

        default:
            fprintf(stderr, "; Unknown node type when dumping: ");
            return;
    }
    debugPrintTree(stdout, node);
    printf("\n");
}

void dumpTreeAsm(double rangeStart, double rangeEnd, Node** functions) {
    constantCount = 0;  // will be increased via addConstant() func
    printf("global A, B, f1, f2, f3, df1, df2, df3\n\n");

    printf("section .text\n");
    for (int i = 0; i < 6; ++i) {
        // function definition starts here
        if (i < 3) {
            printf("f%d:\n", i + 1);
        } else {
            printf("df%d:\n", (i - 3) + 1);
        }

        printf("    push ebp\n");
        printf("    mov ebp, esp\n");

        // recursively dumping AST
        dumpNodeAsm(functions[i]);

        printf("    pop ebp\n");
        printf("    ret\n\n");
    }

    // dumping evaluated constants into section .data
    printf("section .data\n");
    printf("    A   dq %#.15f\n", rangeStart);
    printf("    B   dq %#.15f\n", rangeEnd);
    for (int i = 0; i < constantCount; ++i) {
        printf("    const%d   dq %#.15g\n", i + 1, dataConstants[i]);
    }
    printf("\n");
    printf("; functions.asm\n");
    printf("; Copyright (C) 2026: Yeremey Pervunetskikh\n;\n");
    printf("; This program is free software: you can redistribute it\n");
    printf("; and/or modify it under the terms of the GNU General Public License\n");
}
