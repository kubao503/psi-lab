#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

#define BUFSIZE 1024

int sockfd = -1;

void sigint_handler(int signum)
{
    close(sockfd);
    exit(1);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);
    int port;

    if (argc < 2)
    {
        printf("no port, using 8000\n");
        port = 8000;
    }
    else
    {
        port = atoi(argv[1]);
    }

    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    printf("Will listen on %s : %d\n", inet_ntoa(server_addr.sin_addr), port);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    char buffer[BUFSIZE];
    while (1)
    {
        int bytes_received = recvfrom(sockfd, buffer, BUFSIZE, 0, (struct sockaddr *)&client_addr, &addr_len);

        if (bytes_received <= 0)
        {
            printf("Error in datagram?\n");
            break;
        }

        printf("Received datagram from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        int pair_count, str_len, packet_id;
        memcpy(&pair_count, buffer, sizeof(int));
        memcpy(&str_len, buffer + 4, sizeof(int));
        memcpy(&packet_id, buffer + 8, sizeof(int));
        // printf("%d %d %d\n", pair_count, str_len, packet_id);

        int data_offset = 12;

        char **keys = (char **)malloc(pair_count * sizeof(char *));
        for (int i = 0; i < pair_count; ++i)
        {
            // Calculate the offset for the current key in the buffer
            int offset = data_offset + i * str_len;

            // Allocate memory for the key
            keys[i] = (char *)malloc(str_len);

            // Copy the key from the buffer to the allocated memory
            memcpy(keys[i], buffer + offset, str_len);
        }

        char **values = (char **)malloc(pair_count * sizeof(char *));
        for (int i = 0; i < pair_count; ++i)
        {
            // Calculate the offset for the current key in the buffer
            int offset = data_offset + pair_count * str_len + i * str_len;

            // Allocate memory for the key
            values[i] = (char *)malloc(str_len);

            // Copy the key from the buffer to the allocated memory
            memcpy(values[i], buffer + offset, str_len);
        }

        printf("\n{\n");
        for (int i = 0; i < pair_count; ++i)
        {
            printf("\t%s: %s\n", keys[i], values[i]);
        }
        printf("}\n");

        int send_res = sendto(sockfd, &bytes_received, sizeof(bytes_received), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
        if (send_res == -1)
        {
            perror("sendto");
            printf("Error sending confirmation!\n");
        }

        free(keys);
        free(values);
    }

    if (close(sockfd) == -1)
    {
        printf("Error closing socket!\n");
    }
    return 0;
}
