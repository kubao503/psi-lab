#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_DEPTH 20

struct Node {
    struct Node *child_left;
    struct Node *child_right;
    char *text;
};

struct Node *allocate_node(char *text);
void free_tree(struct Node *node);
struct Node *get_tree_example();

#endif
