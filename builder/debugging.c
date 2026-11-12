#include "debugging.h"
#include "astree.h"

#include <stdio.h>

void debugPrintTree(FILE* out, Node* node) {
    if (!node) {
        return;
    }

    Node *left = node->left, *right = node->right;
    // isSwapped flag is raised only when left and right were swapped, ignore this swap
    if (node->isSwapped) {
        Node* temp = right;
        right = left;
        left = temp;
    }

    switch (node->type) {
        case NODE_NUMBER:
            fprintf(out, "%.2g", node->data.value);
            break;

        case NODE_VARIABLE:
            fprintf(out, "%c", node->data.varName);
            break;

        case NODE_BINARY: {
            char oper = '?';
            if (node->data.op == OP_ADD)
                oper = '+';
            else if (node->data.op == OP_SUB)
                oper = '-';
            else if (node->data.op == OP_MUL)
                oper = '*';
            else if (node->data.op == OP_DIV)
                oper = '/';

            fprintf(out, "(");
            debugPrintTree(out, left);
            fprintf(out, " %c ", oper);
            debugPrintTree(out, right);
            fprintf(out, ")");
            break;
        }

        case NODE_UNARY: {
            const char* oper = "?";
            if (node->data.op == OP_SIN)
                oper = "sin";
            else if (node->data.op == OP_COS)
                oper = "cos";
            else if (node->data.op == OP_TAN)
                oper = "tan";
            else if (node->data.op == OP_CTG)
                oper = "ctg";
            fprintf(out, "%s(", oper);
            debugPrintTree(out, left);
            fprintf(out, ")");
            break;
        }

        default:
            fprintf(out, "[UNKNOWN]");
            break;
    }
}
