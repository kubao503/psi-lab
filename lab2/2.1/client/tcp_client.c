#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void print_buffer(void *buf, size_t buf_len) {
    for (int i = 0; i < buf_len; ++i)
        printf("%d ", ((char *)buf)[i]);
    printf("\n");
}

int connect_to_server() {
    int sfd;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0; /* Any protocol */

    if (getaddrinfo("server", "8000", &hints, &result)) {
        exit(EXIT_FAILURE);
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break; /* Success */

        close(sfd);
    }

    freeaddrinfo(result);

    if (rp == NULL) {
        fprintf(stderr, "Server not found\n");
        exit(EXIT_FAILURE);
    }

    return sfd;
}

int main(int argc, char *argv[]) {
    struct Node *root = get_tree_example();
    size_t buf_len;
    void *buf = convert_tree_to_buf(root, &buf_len);

    print_buffer(buf, buf_len);

    int sfd = connect_to_server();
    if (write(sfd, buf, buf_len) != buf_len) {
        fprintf(stderr, "Data sending fail\n");
        exit(EXIT_FAILURE);
    }

    close(sfd);
    free(buf);
}
