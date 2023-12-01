#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>

#define BUFSIZE 1024
#define STR_LEN 50

int sockfd = -1;

void sigint_handler(int signum)
{
    close(sockfd);
    exit(1);
}

int main(int argc, char *argv[])
{
    signal(SIGINT, sigint_handler);

    struct hostent *host_ent = gethostbyname("server");
    if (host_ent == NULL)
    {
        printf("Err: Server not found\n");
        exit(1);
    }

    struct in_addr *addr = (struct in_addr *)host_ent->h_addr_list[0];
    char *HOST = inet_ntoa(*addr);
    printf("%s\n", HOST);

    int port;

    if (argc < 2)
    {
        printf("no port, using 8000\n");
        port = 8000;
    }
    else
    {
        port = atoi(argv[1]);
        if (port == 0)
        {
            printf("Invalid port number\n");
            exit(2);
        }
    }

    printf("Will send to %s:%d\n", HOST, port);

    struct sockaddr_in server_addr;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        printf("Failed to make socket\n");
        exit(3);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(HOST);
    server_addr.sin_port = htons(port);

    char *keys[] = {"type", "value", "id", "content"};
    char *values[] = {"text", "bajojajo", "1", "docker network inspect mynetwork"};
    int pair_count = sizeof(keys) / sizeof(keys[0]);

    int data_size = 3 * sizeof(int) + pair_count * 2 * STR_LEN;
    char *packed_data = (char *)malloc(data_size);

    int str_len = STR_LEN;
    memcpy(packed_data, &pair_count, sizeof(pair_count));
    memcpy(packed_data + 4, &str_len, sizeof(str_len));

    int offset = 12;

    // Pack all keys
    for (int i = 0; i < pair_count; ++i)
    {
        // Copy the key to packed_data with padding
        memset(packed_data + offset, 0, STR_LEN);
        memcpy(packed_data + offset, keys[i], strlen(keys[i]));

        // Move to the next key offset
        offset += STR_LEN;
    }

    // Pack all values
    for (int i = 0; i < pair_count; ++i)
    {
        // Copy the value to packed_data with padding
        memset(packed_data + offset, 0, STR_LEN);
        memcpy(packed_data + offset, values[i], strlen(values[i]));

        // Move to the next value offset
        offset += STR_LEN;
    }

    for (int packet_id = 0; packet_id < 5; ++packet_id)
    {
        memcpy(packed_data + 8, &packet_id, sizeof(packet_id));

        int res = sendto(sockfd, packed_data, data_size, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        if (res == -1)
        {
            perror("sendto");
            printf("Error occured!");
        }
        else
        {
            printf("\nSent package %d of size %d bytes \n", packet_id, res);

            int response_size = 2 * sizeof(int);
            char response[response_size];
            int recv_res = recvfrom(sockfd, &response, sizeof(response), 0, NULL, NULL);

            int bytes_received, recv_packed_id;
            memcpy(&bytes_received, response, sizeof(int));
            memcpy(&recv_packed_id, response + 4, sizeof(int));

            if (recv_res == -1)
            {
                perror("recvfrom");
                printf("Error receiving confirmation!\n");
            }

            if (data_size == bytes_received)
            {
                printf("Package %d sent successfully\n", recv_packed_id);
            }
            else
            {
                printf("Error: Received different number of bytes! %d != %d\n", sizeof(packed_data), bytes_received);
            }
        }
    }

    if (close(sockfd) == -1)
    {
        printf("Error closing socket!\n");
    }
    return 0;
}
