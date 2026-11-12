#ifndef _ASTREE_H
#define _ASTREE_H

/* This file ensures Abstract Syntax Tree semantics for building, parsing,
 * optimizing and dumping. Mainly it provides primitives for operates Nodes.
 */

/// Type of the AST node
typedef enum {
    NODE_NUMBER,    /// floating-point number or constant
    NODE_VARIABLE,  /// variable X
    NODE_BINARY,    /// binary operator
    NODE_UNARY      /// unary operator
} NodeType;

/// Type of the operator
typedef enum {
    /// binary operators:
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    /// unary ones:
    OP_SIN,
    OP_COS,
    OP_TAN,
    OP_CTG
} OpType;

/// AST node structure
typedef struct Node {
    NodeType type;  /// node category type

    union {
        double value;  /// numeric value for NODE_NUMBER
        char varName;  /// variable name for NODE_VARIABLE
        OpType op;     /// operator type for OP nodes
    } data;            /// node data

    struct Node* left;   /// left child
    struct Node* right;  /// right child (NULL for unary)

    char isSwapped;  /// used for optimized sub and div instructions
                     /// when right and left were changed

    char isDuplicated;  /// used for optimized square multiplication
                        /// when right and left are equal
} Node;

/// Allocates memory for primitive constant
Node* createNumberNode(double val);

/// Allocates memory for variable node
Node* createVarNode(char var);

/// Allocates memory for operator node
Node* createOpNode(OpType oper, NodeType type, Node* left, Node* right);

/// Recursively clean up the tree, with root @param root
void freeTree(Node* root);

/// Recursively clones the tree with allocates memory
Node* cloneTree(Node* root);

#endif
