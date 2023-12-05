#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct Node *get_tree_example() {
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

size_t get_tree_size_rec(struct Node *node, size_t total_size) {
    if (!node)
        return total_size;

    total_size = get_tree_size_rec(node->child_left, total_size);
    total_size = get_tree_size_rec(node->child_right, total_size);
    return total_size + 3 * sizeof(unsigned) + strlen(node->text);
}

size_t get_tree_size(struct Node *root) {
    return get_tree_size_rec(root, 0);
}

int write_tree_to_buf_rec(struct Node *node, void **buf, int *index) {
    if (!node)
        return -1;

    int index_left = write_tree_to_buf_rec(node->child_left, buf, index);
    int index_right = write_tree_to_buf_rec(node->child_right, buf, index);

    size_t str_len = strlen(node->text);
    memcpy(*buf, &str_len, 4);
    memcpy(*buf + 4, node->text, str_len);

    memcpy(*buf + 4 + str_len, &index_left, 4);
    memcpy(*buf + 8 + str_len, &index_right, 4);

    for (int i = 0; i < 12 + str_len; ++i)
        printf("%d ", ((char *)*buf)[i]);
    printf("\n");

    *buf += 12 + str_len;
    *index += 1;
    return *index;
}

void write_tree_to_buf(struct Node *root, void *buf) {
    int index = -1;
    write_tree_to_buf_rec(root, &buf, &index);
}

int main(int argc, char *argv[]) {
    struct Node *root = get_tree_example();

    print_tree_paths(root);
    size_t tree_size = get_tree_size(root);
    void *buf = malloc(tree_size);
    write_tree_to_buf(root, buf);

    for (int i = 0; i < tree_size; ++i)
        printf("%d ", ((char *)buf)[i]);
    printf("\n");

    free(buf);
}
