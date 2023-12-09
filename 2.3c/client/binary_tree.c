#include "binary_tree.h"

struct Node *allocate_node(char *text) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->child_left = NULL;
    node->child_left = NULL;
    node->text = text;
}

void free_tree(struct Node *node) {
    if (!node) {
        return;
    }
    free_tree(node->child_left);
    free_tree(node->child_right);
    free(node->child_left);
    free(node->child_right);
}

void print_tree_paths_rec(struct Node *node, char *path_texts[], int path_texts_len) {
    if (!node)
        return;

    path_texts[path_texts_len] = node->text;

    if (!node->child_left && !node->child_right) {
        for (int i = path_texts_len; i >= 0; --i)
            printf("%s", path_texts[i]);
        printf("\n");
        return;
    }

    print_tree_paths_rec(node->child_left, path_texts, path_texts_len + 1);
    print_tree_paths_rec(node->child_right, path_texts, path_texts_len + 1);
}

void print_tree_paths(struct Node *root) {
    char *path_texts[MAX_DEPTH];
    print_tree_paths_rec(root, path_texts, 0);
}

struct Node *get_tree_example() {
    struct Node *root = allocate_node(".");
    root->child_left = allocate_node(".com");
    root->child_left->child_left = allocate_node(".google");
    root->child_right = allocate_node(".pl");
    root->child_right->child_left = allocate_node(".edu");
    root->child_right->child_left->child_left = allocate_node(".pw");
    root->child_right->child_left->child_left->child_left = allocate_node(".elka");
    root->child_right->child_left->child_left->child_left->child_left = allocate_node(".studia");
    root->child_right->child_left->child_left->child_left->child_right = allocate_node(".mion");

    print_tree_paths(root);

    return root;
}
