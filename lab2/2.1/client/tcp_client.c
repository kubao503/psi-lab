#include <stdio.h>
#include <stdlib.h>

struct Node {
    struct Node *child_left;
    struct Node *child_right;
    char *text;
};

struct Node *allocate_node(char *text) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->child_left = NULL;
    node->child_left = NULL;
    node->text = text;
}

struct Node *get_binary_tree() {
    struct Node *root = allocate_node(".");
    root->child_left = allocate_node("com");

    return root;
}

int main(int argc, char *argv[]) {
    struct Node *root = get_binary_tree();

    printf("%s %s\n", root->text, root->child_left->text);
}
