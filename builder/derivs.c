#include "derivs.h"
#include "astree.h"

#include <stdlib.h>

/// Cloning AST of some function
/// @param root provided for function
/// @return deriv of such transformation
Node* buildDerive(Node* root) {
    if (!root)
        return NULL;

    switch (root->type) {
        case NODE_NUMBER:
            // C' = 0
            return createNumberNode(0.0);

        case NODE_VARIABLE:
            // x' = 1
            return createNumberNode(1.0);

        case NODE_BINARY: {
            Node* u = root->left;
            Node* v = root->right;
            Node* du = buildDerive(u);
            Node* dv = buildDerive(v);

            switch (root->data.op) {
                case OP_ADD:
                    // (u + v)' = u' + v'
                    return createOpNode(OP_ADD, NODE_BINARY, du, dv);

                case OP_SUB:
                    // (u - v)' = u' - v'
                    return createOpNode(OP_SUB, NODE_BINARY, du, dv);

                case OP_MUL: {
                    // (u * v)' = u' * v + u * v'
                    Node* term1 = createOpNode(OP_MUL, NODE_BINARY, du, cloneTree(v));
                    Node* term2 = createOpNode(OP_MUL, NODE_BINARY, cloneTree(u), dv);
                    return createOpNode(OP_ADD, NODE_BINARY, term1, term2);
                }

                case OP_DIV: {
                    // (u / v)' = (u' * v - u * v') / (v * v)
                    Node* term1 = createOpNode(OP_MUL, NODE_BINARY, du, cloneTree(v));
                    Node* term2 = createOpNode(OP_MUL, NODE_BINARY, cloneTree(u), dv);
                    Node* num = createOpNode(OP_SUB, NODE_BINARY, term1, term2);
                    Node* denom =
                        createOpNode(OP_MUL, NODE_BINARY, cloneTree(v), cloneTree(v));
                    return createOpNode(OP_DIV, NODE_BINARY, num, denom);
                }

                default:
                    return NULL;
            }
        }

        case NODE_UNARY: {
            Node* u = root->left;
            Node* du = buildDerive(u);

            switch (root->data.op) {
                case OP_SIN: {
                    // (sin(u))' = cos(u) * u'
                    Node* cosNode = createOpNode(OP_COS, NODE_UNARY, cloneTree(u), NULL);
                    return createOpNode(OP_MUL, NODE_BINARY, cosNode, du);
                }
                case OP_COS: {
                    // (cos(u))' = (0 - sin(u)) * u'
                    Node* sinNode = createOpNode(OP_SIN, NODE_UNARY, cloneTree(u), NULL);
                    Node* zeroNode = createNumberNode(0.0);
                    Node* negSinNode =
                        createOpNode(OP_SUB, NODE_BINARY, zeroNode, sinNode);
                    return createOpNode(OP_MUL, NODE_BINARY, negSinNode, du);
                }
                case OP_TAN: {
                    // (tan(u))' = (1 / (cos(u) * cos(u))) * u'
                    Node* cosNode1 = createOpNode(OP_COS, NODE_UNARY, cloneTree(u), NULL);
                    Node* cosNode2 = cloneTree(cosNode1);
                    Node* cosSq = createOpNode(OP_MUL, NODE_BINARY, cosNode1, cosNode2);

                    Node* oneNode = createNumberNode(1.0);
                    Node* secSq = createOpNode(OP_DIV, NODE_BINARY, oneNode, cosSq);
                    return createOpNode(OP_MUL, NODE_BINARY, secSq, du);
                }
                case OP_CTG: {
                    // (ctg(u))' = (0 - 1 / (sin(u) * sin(u))) * u'
                    Node* sinNode1 = createOpNode(OP_SIN, NODE_UNARY, cloneTree(u), NULL);
                    Node* sinNode2 = cloneTree(sinNode1);
                    Node* sinSq = createOpNode(OP_MUL, NODE_BINARY, sinNode1, sinNode2);

                    Node* oneNode = createNumberNode(1.0);
                    Node* divNode = createOpNode(OP_DIV, NODE_BINARY, oneNode, sinSq);

                    Node* zeroNode = createNumberNode(0.0);
                    Node* negCscSq = createOpNode(OP_SUB, NODE_BINARY, zeroNode, divNode);
                    return createOpNode(OP_MUL, NODE_BINARY, negCscSq, du);
                }

                default:
                    return NULL;
            }
        }

        default:
            return NULL;
    }
}
