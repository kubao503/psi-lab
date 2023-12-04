#include <stdio.h>
#include <stdlib.h>

#define MAX_DEPTH 20

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
    struct Node *root = allocate_node("");
    root->child_left = allocate_node(".com");
    root->child_right = allocate_node(".pl");
    root->child_right->child_left = allocate_node(".edu");
    root->child_right->child_left->child_left = allocate_node(".pw");

    return root;
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

int main(int argc, char *argv[]) {
    struct Node *root = get_binary_tree();

    print_tree_paths(root);
}
