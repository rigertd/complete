#ifndef DTREE_H
#define DTREE_H

/* Represents a decision tree.
 */
typedef struct _DTree DTree;

/* Represents a node in the decision tree.
 */
typedef struct _Node Node;

/*  Creates a new decision tree.
 */
DTree *newDTree();

/* Deallocates all nodes in the specified decision tree.
 * pre: tree is not null
 * post: tree is empty
 */
void clearDTree(DTree *tree);

/* Deletes the specified decision tree.
 * pre: tree is not null
 * post: tree is null
 */
void deleteDTree(DTree *tree);

/* Determines whether the specified node is a leaf (answer) node.
 * pre: node is not null
 */
int isLeaf(Node *node);

/* Prompts the user for the correct answer and a question that differentiates
 * the current answer from the actual answer, and returns a subtree to replace
 * current.
 */
Node *addInterior(Node *current);

#endif