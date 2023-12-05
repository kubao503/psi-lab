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
    return 4 + get_tree_size_rec(root, 0);
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

    // for (int i = 0; i < 12 + str_len; ++i)
    //     printf("%d ", ((char *)*buf)[i]);
    // printf("\n");

    *buf += 12 + str_len;
    return (*index)++;
}

void write_tree_to_buf(struct Node *root, void *buf) {
    int index = 0;
    void *buf_ptr = buf + 4;
    write_tree_to_buf_rec(root, &buf_ptr, &index);
    memcpy(buf, &index, 4);
}

void *convert_tree_to_buf(struct Node *root, size_t *buf_len) {
    size_t tree_size = get_tree_size(root);
    *buf_len = tree_size;
    void *buf = malloc(tree_size);
    write_tree_to_buf(root, buf);
    return buf;
}

void test() {
    struct Node *root = allocate_node("");
    root->child_left = allocate_node(".com");
    root->child_right = allocate_node(".pl");
    root->child_right->child_left = allocate_node(".edu");
    root->child_right->child_left->child_left = allocate_node(".pw");

    size_t buf_len;
    void *buf = convert_tree_to_buf(root, &buf_len);

    if (buf_len != 78) {
        printf("Buffer length don't match\n");
        exit(1);
    }
    int expected[] = {5, 0, 0, 0, 4, 0, 0, 0, 46, 99, 111, 109, -1, -1, -1, -1, -1, -1, -1, -1, 3, 0, 0, 0, 46, 112, 119, -1, -1, -1, -1, -1, -1, -1, -1, 4, 0, 0, 0, 46, 101, 100, 117, 1, 0, 0, 0, -1, -1, -1, -1, 3, 0, 0, 0, 46, 112, 108, 2, 0, 0, 0, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0};
    for (int i = 0; i < buf_len; ++i) {
        char c = *((char *)(buf + i));
        if (c != expected[i]) {
            printf("Result at index %d don't match %d != %d\n", i, c, expected[i]);
            exit(1);
        }
    }
    printf("Test successfull\n");

    free(buf);
}

int main(int argc, char *argv[]) {
    struct Node *root = get_tree_example();
    size_t buf_len;
    void *buf = convert_tree_to_buf(root, &buf_len);

    // for (int i = 0; i < buf_len; ++i)
    //     printf("%d ", ((char *)buf)[i]);
    // printf("\n");

    test();

    free(buf);
}
