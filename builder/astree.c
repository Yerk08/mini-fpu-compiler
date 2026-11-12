#include "astree.h"

#include <stdlib.h>
#include <string.h>

Node* createNumberNode(double val) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node)
        return NULL;
    node->type = NODE_NUMBER;
    node->data.value = val;
    node->left = NULL;
    node->right = NULL;
    node->isSwapped = 0;
    node->isDuplicated = 0;
    return node;
}

Node* createVarNode(char var) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node)
        return NULL;
    node->type = NODE_VARIABLE;
    node->data.varName = var;
    node->left = NULL;
    node->right = NULL;
    node->isSwapped = 0;
    node->isDuplicated = 0;
    return node;
}

Node* createOpNode(OpType oper, NodeType type, Node* left, Node* right) {
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node)
        return NULL;
    node->type = type;
    node->data.op = oper;
    node->left = left;
    node->right = right;
    node->isSwapped = 0;
    node->isDuplicated = 0;
    return node;
}

void freeTree(Node* root) {
    if (!root)
        return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

Node* cloneTree(Node* root) {
    if (!root)
        return NULL;

    Node* node = (Node*)malloc(sizeof(Node));
    if (!node)
        return NULL;

    node->type = root->type;
    node->data = root->data;
    node->left = cloneTree(root->left);
    node->right = cloneTree(root->right);
    node->isSwapped = root->isSwapped;
    node->isDuplicated = root->isDuplicated;

    return node;
}
