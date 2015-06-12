#include "dtree.h"

struct _DTree {
    Node *root;
};

struct _Node {
    char *val;
    Node *no;
    Node *yes;
};

/*  Creates a new decision tree.
 */
DTree *newDTree() {
    DTree *dt = (DTree *)malloc(sizeof(DTree));
    assert(dt != 0);
    dt->root = 0;
    
    return dt;
}

/*  Initializes the specified decision tree with the specified value as the root node.
 *  pre: tree is not null
 *  pre: tree is empty
 *  post: tree has a root node with the specified answer value.
 */
void initDTree(DTree *tree, char *val) {
    assert(tree != 0);
    assert(tree->root == 0);
    
    Node *n = (Node *)malloc(sizeof(Node));
    assert(n != 0);
    n->val = val;
}

/* Deallocates all nodes in the specified decision tree.
 * pre: tree is not null
 * post: tree is empty
 */
void clearDTree(DTree *tree) {
    assert(tree != 0);
    assert(tree->root != 0);
    
    tree->root = _deleteNode(tree->root);
}

Node *_deleteNode(Node *current) {
    if (current == 0)
        return 0;
    if (current->no)
        current->no = _deleteNode(current->no);
    if (current->yes)
        current->yes = _deleteNode(current->yes);
    free(current);
    return 0;
}

/* Deletes the specified decision tree.
 * pre: tree is not null
 * post: tree is null
 */
void deleteDTree(DTree *tree) {
    assert(tree != 0);
    if (tree->root)
        clearDTree(tree);
    free(tree);
}

/* Determines whether the specified node is a leaf (answer) node.
 * pre: node is not null
 */
int isLeaf(Node *node) {
    assert(node != 0);
    return !(node->no || node->yes);
}

/* Prompts the user for the correct answer and a question that differentiates
 * the current answer from the actual answer, and returns a subtree to replace
 * current.
 */
Node *addInterior(Node *current) {
    char *yes = (char *)malloc(sizeof(char) * 81);
    char *question = (char *)malloc(sizeof(char) * 81);
    printf("What animal were you thinking of?\n> ");
    fgets(yes, 80, stdin);
    yes = strtok(yes, "\n");
    printf("Type a yes/no question that leads to a %s instead of a %s\n> ", yes, current->val);
    fgets(question, 80, stdin);
    question = strtok(question, "\n");
    
    Node *yn = (Node *)malloc(sizeof(Node));
    assert(yn != 0);
    Node *qn = (Node *)malloc(sizeof(Node));
    assert(qn != 0);
    
    yn->val = yes;
    qn->val = question;
    
    qn->yes = yn;
    qn->no = current;
    
    return qn;
}
