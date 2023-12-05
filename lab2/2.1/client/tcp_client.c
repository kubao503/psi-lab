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

struct addrinfo *get_hosts(char *host_name, char *port) {
    struct addrinfo hints;
    struct addrinfo *hosts;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0; /* Any protocol */

    if (getaddrinfo(host_name, port, &hints, &hosts)) {
        fprintf(stderr, "getaddrinfo failed\n");
        exit(EXIT_FAILURE);
    }

    return hosts;
}

int connect_to_host(struct addrinfo *hosts) {
    int sfd;
    struct addrinfo *host_ptr;

    for (host_ptr = hosts; host_ptr != NULL; host_ptr = host_ptr->ai_next) {
        sfd = socket(host_ptr->ai_family, host_ptr->ai_socktype,
                     host_ptr->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, host_ptr->ai_addr, host_ptr->ai_addrlen) != -1)
            break; /* Success */

        close(sfd);
    }

    if (host_ptr == NULL) {
        fprintf(stderr, "Server not found\n");
        exit(EXIT_FAILURE);
    }

    return sfd;
}

void send_buf(int sfd, void *buf, size_t buf_len) {
    if (write(sfd, buf, buf_len) != buf_len) {
        fprintf(stderr, "Data sending fail\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    struct Node *root = get_tree_example();
    size_t buf_len;
    void *buf = convert_tree_to_buf(root, &buf_len);
    free_tree(root);

    print_buffer(buf, buf_len);

    char *host_name = argv[1];
    char *port = argv[2];
    if (argc < 3) {
        host_name = "server";
        port = "8000";
    }

    printf("Will send to %s:%s\n", host_name, port);

    struct addrinfo *hosts = get_hosts(host_name, port);
    int sfd = connect_to_host(hosts);
    freeaddrinfo(hosts);

    send_buf(sfd, buf, buf_len);

    close(sfd);
    free(buf);
}
