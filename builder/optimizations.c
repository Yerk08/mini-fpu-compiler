#include "optimizations.h"
#include "astree.h"

#include <math.h>
#include <stdlib.h>

/// CONSTANT PROPAGATION BLOCK OCCURS HERE

/// Constant propagation with @param root, @param left child, and @param right one
/// @return updated root node with single constant (or invalid operation set)
static Node* binaryConstantOptimize(Node* root, Node* left, Node* right) {
    double res = 0.0;
    switch (root->data.op) {
        case OP_ADD:
            res = left->data.value + right->data.value;
            break;
        case OP_SUB:
            res = left->data.value - right->data.value;
            break;
        case OP_MUL:
            res = left->data.value * right->data.value;
            break;
        case OP_DIV:
            if (fabs(right->data.value) > 1e-15) {  // right value not zero
                res = left->data.value / right->data.value;
            } else {
                return root;
            }
            break;
        default:
            return root;
    }

    freeTree(root);
    return createNumberNode(res);
}

/// Unary op. constant propagation with @param root, and single child @param left
/// @return updated root node with single constant (or invalid operation set)
static Node* unaryConstantOptimize(Node* root, Node* left) {
    double res = 0.0;

    switch (root->data.op) {
        case OP_SIN:
            res = sin(left->data.value);
            break;
        case OP_COS:
            res = cos(left->data.value);
            break;
        case OP_TAN:
            res = tan(left->data.value);
            break;
        case OP_CTG:
            if (fabs(sin(left->data.value)) > 1e-15) {  // right value not zero
                res = 1.0 / tan(left->data.value);
            } else {
                return root;
            }
            break;
        default:
            return root;
    }

    free(root);
    return createNumberNode(res);
}

/// Function for global constant propagation of entire tree with @param root node
Node* optimizeConstants(Node* root) {
    if (!root)
        return NULL;

    root->left = optimizeConstants(root->left);
    root->right = optimizeConstants(root->right);

    if (root->type == NODE_BINARY) {
        Node* left = root->left;
        Node* right = root->right;

        if (left && left->type == NODE_NUMBER && right && right->type == NODE_NUMBER) {
            return binaryConstantOptimize(root, left, right);
        }
    }

    if (root->type == NODE_UNARY) {
        Node* left = root->left;

        if (left && left->type == NODE_NUMBER) {
            return unaryConstantOptimize(root, left);
        }
    }

    return root;
}

/// STACK OPTIMIZATION BLOCK OCCURS HERE

/// Returns tree height and improves FPU stack usage
int optimizeFpuStack(Node* root) {
    if (!root)
        return 0;

    int leftHeight = optimizeFpuStack(root->left);
    int rightHeight = optimizeFpuStack(root->right);

    if (root->type == NODE_BINARY && rightHeight > leftHeight) {
        // swapping nodes, because right is longer than left one
        Node* temp = root->left;
        root->left = root->right;
        root->right = temp;

        root->isSwapped ^= 1;  // change swapped state of node

        // replaces nodes heights for evaluation
        leftHeight ^= rightHeight;
        rightHeight ^= leftHeight;
        leftHeight ^= rightHeight;
    }
    return (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

/// REDUCTION BLOCK OCCURS HERE

/// Checks that @param node is exactly the value
static int isConstant(Node* node, double val) {
    return (node->type == NODE_NUMBER && fabs(node->data.value - val) < 1e-15);
}

/// Checks that @param left is same to @param right
static int isSame(Node* left, Node* right) {
    if (left->type != right->type)
        return 0;
    switch (left->type) {
        case NODE_NUMBER:
            return fabs(left->data.value - right->data.value) < 1e-15;
        case NODE_VARIABLE:
            return left->data.varName == right->data.varName;
        case NODE_UNARY:
            return left->data.op == right->data.op && isSame(left->left, right->left);
        case NODE_BINARY:
            if (left->data.op != right->data.op)
                return 0;

            if (left->data.op == OP_ADD || left->data.op == OP_MUL) {
                return (isSame(left->left, right->left) &&
                        isSame(left->right, right->right)) ||
                       (isSame(left->left, right->right) &&
                        isSame(left->right, right->left));
            } else {
                return left->data.op == right->data.op &&
                       isSame(left->left, right->left) &&
                       isSame(left->right, right->right);
            }
        default:
            return 0;
    }
}

/// Optimizes multiplication operation in Node @param root
static Node* multiplyOptimize(Node* root, Node* left, Node* right) {
    // Identity: 0 * u = 0
    if (isConstant(left, 0.0)) {
        freeTree(root);
        return createNumberNode(0.0);
    }
    // Identity: u * 1 = u
    if (isConstant(right, 1.0)) {
        free(right);
        free(root);
        return left;
    }
    // Identity: (k * u) * v = k * (u * v)  <- used for complex optimizations
    if (left->type == NODE_BINARY && left->data.op == OP_MUL &&
        left->left->type == NODE_NUMBER) {
        Node* k = left->left;
        Node* u = left->right;
        Node* v = right;
        left->left = u;
        left->right = v;
        free(root);
        return createOpNode(OP_MUL, NODE_BINARY, k, left);
    }
    return root;
}

/// Optimizes add operation in Node @param root
static Node* addOptimize(Node* root, Node* left, Node* right) {
    // Identity: u + 0 = u
    if (isConstant(right, 0.0)) {
        free(right);
        free(root);
        return left;
    }
    // Identity: u + u = 2 * u
    if (isSame(left, right)) {
        free(root);
        freeTree(left);
        Node* twoNode = createNumberNode(2.0);
        return createOpNode(OP_MUL, NODE_BINARY, twoNode, right);
    }
    // Identity: u + k * u = (k + 1) * u
    if (right->type == NODE_BINARY && right->data.op == OP_MUL &&
        isSame(left, right->right) && right->left->type == NODE_NUMBER) {
        Node* temp = right;
        temp->left->data.value += 1;
        free(root);
        freeTree(left);
        temp->isDuplicated = 0;
        return temp;
    }
    return root;
}

/// Optimizes sub operation in Node @param root
static Node* subOptimize(Node* root, Node* left, Node* right) {
    // Identity: u - 0 = u
    if (isConstant(right, 0.0)) {
        free(right);
        free(root);
        return left;
    }
    // Identity: u - u = 0
    if (isSame(left, right)) {
        freeTree(root);
        return createNumberNode(0.0);
    }
    return root;
}

/// Optimizes div operation in Node @param root
static Node* divOptimize(Node* root, Node* left, Node* right) {
    // Identity: 0 / v = 0
    if (isConstant(left, 0.0)) {
        freeTree(root);
        return createNumberNode(0.0);
    }
    // Identity: u / 1 = u
    if (isConstant(right, 1.0)) {
        free(right);
        free(root);
        return left;
    }
    // Identity: u / u = 1.0
    if (isSame(left, right)) {
        freeTree(root);
        return createNumberNode(1.0);
    }
    return root;
}

/// Function for evaluation reduction of the tree with @param root node
Node* optimizeReduction(Node* root) {
    if (!root)
        return NULL;

    root->left = optimizeReduction(root->left);
    root->right = optimizeReduction(root->right);

    if (root->type == NODE_BINARY) {
        Node* left = root->left;
        Node* right = root->right;

        switch (root->data.op) {
            case OP_MUL: {
                Node* node = multiplyOptimize(root, left, right);
                if (node != root)
                    return node;

                return multiplyOptimize(root, right, left);
                break;
            }
            case OP_ADD: {
                Node* node = addOptimize(root, left, right);
                if (node != root)
                    return node;

                return addOptimize(root, right, left);
                break;
            }
            case OP_SUB: {
                return subOptimize(root, left, right);
                break;
            }
            case OP_DIV: {
                return divOptimize(root, left, right);
                break;
            }
            default:
                break;
        }
    }

    return root;
}

/// Function for square reduction for @param root node
void optimizeHintsSquares(Node* root) {
    if (!root)
        return;

    optimizeHintsSquares(root->left);
    optimizeHintsSquares(root->right);

    if (root->type == NODE_BINARY) {
        Node* left = root->left;
        Node* right = root->right;

        switch (root->data.op) {
            case OP_MUL: {
                if (isSame(left, right))
                    root->isDuplicated = 1;
                break;
            }
            default:
                break;
        }
    }
}
