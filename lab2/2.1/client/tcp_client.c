#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "binary_tree.h"

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

    size_t tree_size = get_tree_size(root);
    void *buf = malloc(tree_size);
    write_tree_to_buf(root, buf);

    for (int i = 0; i < tree_size; ++i)
        printf("%d ", ((char *)buf)[i]);
    printf("\n");

    free(buf);
}
