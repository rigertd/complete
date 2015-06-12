#include <stdio.h>
#include <string.h>
#include "dtree.h"

#define MAX_STRING 256

int main() {
    FILE *file = fopen("animal.dat", "r");
    if (file == 0)
    {
        printf("Error loading animal.dat.\n");
        exit(1);
    }
    
    DTree *tree = loadTree(file);
}

int ask(Node *cur) {
    assert(cur != 0);
    char c;
    if (isLeaf(cur)) {
        printf("Are you thinking of a %s? (y/n)\n> ", cur->val);
    } else {
        printf("%s (y/n)\n>", cur->val);
    }
    scanf("%c", &c);
    while (c != 'y' && c != 'Y' && c != 'n' && c != 'N') {
        printf("Invalid response. Enter 'y' or 'n'.");
        scanf("%c", &c);
    }
    if (c == 'y' || c == 'Y') {
        printf("You were thinking of a %s! Good game!\n");
        return 1;
    } else {
        return 0;
    }
}

/*  Loads the data from the specified file pointer,
    saves it in a new DTree, and returns a pointer to the tree.
    
    pre: fs is not null and is readable
 */
DTree *loadTree(FILE *fs) {
    /* verify precondition */
    assert(fs != 0);
    
    /* create and initialize a DTree */
    DTree t = newDTree();
    
    /* buffer for reading from file */
    char buffer[MAX_STRING];
    
    /* read from file and add to DTree until EOF is reached */
    t->root = readNode(buffer, fs);
}

Node *readNode(char *buf, FILE *fs) {
    assert(fs != 0);
    
    Node *cur;
    
    fgets(buf, MAX_STRING, fs);
    buf = strtok(buf, "\n");
    if (buf == EOF || strcmp(buf, "#") == 0 || strlen(buf) < 1)
        return 0;

    cur = (Node *)malloc(sizeof(Node));
    cur->val = buf;
    cur->no = readNode(buf, fs);
    cur->yes = readNode(buf, fs);
    return cur;
}
