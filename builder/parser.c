#include "parser.h"
#include "astree.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* openSpecFile(void) {
    char* filepath = getenv("SPEC_FILE");
    if (!filepath) {
        fprintf(stderr, "SPEC_FILE environment variable was not set\n");
        return NULL;
    }

    FILE* file = fopen(filepath, "r");
    if (!file) {
        fprintf(stderr, "Cannot open file %s\n", filepath);
    }
    return file;
}

Node* parseLine(char* line) {
    Node* stack[256];  // stack for reading reverse-polish notion
    int top = 0;       // pointer to the top Node

    char* token = strtok(line, " \t\r\n");
    while (token != NULL) {
        double val;

        // appending nodes directly to stack:
        if (strcmp(token, "x") == 0) {
            stack[top++] = createVarNode('x');

        } else if (strcmp(token, "pi") == 0) {
            stack[top++] = createNumberNode(M_PI);

        } else if (strcmp(token, "e") == 0) {
            stack[top++] = createNumberNode(M_E);

        } else if (strcmp(token, "+") == 0 || strcmp(token, "-") == 0 ||
                   strcmp(token, "*") == 0 || strcmp(token, "/") == 0) {

            // operates with 2 top nodes and combines its to single one
            if (top < 2) {
                fprintf(stderr, "Binary operator '%s' requires 2 operands\n", token);
                freeTree(stack[--top]);
                return NULL;
            }

            Node* right = stack[--top];
            Node* left = stack[--top];
            OpType oper = (token[0] == '+')   ? OP_ADD
                          : (token[0] == '-') ? OP_SUB
                          : (token[0] == '*') ? OP_MUL
                                              : OP_DIV;
            // new node always push down to the stack
            stack[top++] = createOpNode(oper, NODE_BINARY, left, right);

        } else if (strcmp(token, "sin") == 0 || strcmp(token, "cos") == 0 ||
                   strcmp(token, "tan") == 0 || strcmp(token, "ctg") == 0) {

            // operates with only one top node and wraps it to new one
            if (top < 1) {
                fprintf(stderr, "Unary operator '%s' requires 1 operand\n", token);
                return NULL;
            }

            Node* left = stack[--top];
            OpType oper = (strcmp(token, "sin") == 0)   ? OP_SIN
                          : (strcmp(token, "cos") == 0) ? OP_COS
                          : (strcmp(token, "tan") == 0) ? OP_TAN
                                                        : OP_CTG;
            // new node always push down to the stack
            stack[top++] = createOpNode(oper, NODE_UNARY, left, NULL);

        } else if (sscanf(token, "%lf", &val) == 1) {
            stack[top++] = createNumberNode(val);  // constant were set

        } else {
            fprintf(stderr, "Unknown token '%s'\n", token);
            while (top > 0)
                freeTree(stack[--top]);
            return NULL;
        }

        token = strtok(NULL, " \t\r\n");
    }

    if (top > 1) {
        fprintf(stderr, "Multiple roots left on stack\n");
        while (top >= 0)
            freeTree(stack[--top]);
        return NULL;

    } else if (top == 0) {
        fprintf(stderr, "No found operands in the string\n");
        return NULL;
    }

    return stack[0];  // returns only one Node - the entire tree
}

int processFileLines(FILE* file, double* rangeStart, double* rangeEnd, Node** functions) {
    /* SPEC_FILE format:
     * -6.1 (A boundary)
     * 15.2 (B boundary)
     * 2 x 4 / tan -  (f1 in reverse-polish notion)
     * x x /          (f2 function)
     * 0.2 pi *       (f3 function - may be even constant)
     */

    if (fscanf(file, "%lf %lf", rangeStart, rangeEnd) != 2) {
        fprintf(stderr, "Failed to read range boundaries from file\n");
        return 1;
    }

    char line[1024];
    // consume newline from the first line
    fgets(line, sizeof(line), file);

    int index = 0;
    while (index < 3 && fgets(line, sizeof(line), file)) {
        if (line[0] == '\n' || line[0] == '\r' || line[0] == '\0') {
            continue;
        }

        functions[index] = parseLine(line);
        if (!functions[index]) {
            fprintf(stderr, "Failed to parse function at line %d\n", index + 2);
            return 1;
        }
        ++index;
    }

    if (index < 3) {
        fprintf(stderr, "Expected 3 functions in file, but found only %d\n", index);
        return 1;
    }

    return 0;
}

/// Main combination of functions above this one
int loadSpecification(double* rangeStart, double* rangeEnd, Node** functions) {
    for (int i = 0; i < 3; i++) {
        functions[i] = NULL;  // for safety freeTree() in cycle below
    }

    FILE* file = openSpecFile();
    if (!file) {
        return 1;
    }

    int result = processFileLines(file, rangeStart, rangeEnd, functions);
    fclose(file);

    if (result != 0) {  // error was occured
        for (int i = 0; i < 3; i++) {
            if (functions[i] != NULL) {
                freeTree(functions[i]);
                functions[i] = NULL;
            }
        }
        return 1;
    }

    return 0;
}
