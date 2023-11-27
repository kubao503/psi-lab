#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 1024
#define STR_LEN 50

int main(int argc, char *argv[])
{
  char *HOST = "172.18.0.2";
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

  printf("Will send to %s:%d\n", HOST, port);

  struct sockaddr_in server_addr;
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = inet_addr(HOST);
  server_addr.sin_port = htons(port);

  char *keys[] = {"type", "value", "id", "content"};
  char *values[] = {"text", "bajojajo", "1", "docker network inspect mynetwork"};
  char pair_count = sizeof(keys) / sizeof(keys[0]);

  char *packed_data = (char *)malloc(2 * sizeof(int) + pair_count * 2 * STR_LEN);

  packed_data[3] = pair_count;
  packed_data[7] = STR_LEN;

  int offset = 8;

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

  for (int i = 0; i < 5; ++i)
  {
    int res = sendto(sockfd, packed_data, 2 * sizeof(int) + pair_count * STR_LEN * 2, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (res == -1)
    {
      perror("sendto");
      printf("Error occured!");
    }
    else
    {
      printf("\nSent %d bytes of data.", res);
    }
  }

  close(sockfd);
  return 0;
}
