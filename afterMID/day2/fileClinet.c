#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in server_addr;

    char buffer[BUFFER_SIZE];
    char filename[256];

    if (argc != 3)
    {
        printf("Usage: %s <server-ip> <port>\n", argv[0]);
        return 1;
    }

    char *server_ip = argv[1];
    int port = atoi(argv[2]);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("Socket failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET,
                  server_ip,
                  &server_addr.sin_addr) <= 0)
    {
        perror("Invalid IP");
        close(sock);
        return 1;
    }

    if (connect(sock,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    printf("Connected to server.\n\n");

    printf("Files available on server:\n");

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        int n = recv(sock,
                     buffer,
                     sizeof(buffer) - 1,
                     0);

        if (n <= 0)
        {
            printf("Server disconnected.\n");
            close(sock);
            return 0;
        }

        buffer[n] = '\0';

        printf("%s", buffer);

        if (strstr(buffer, "END_LIST") != NULL)
            break;
    }

    printf("\nEnter file name to download: ");

    scanf("%255s", filename);

    send(sock,
         filename,
         strlen(filename),
         0);

    memset(buffer, 0, sizeof(buffer));

    int n = recv(sock,
                 buffer,
                 sizeof(buffer) - 1,
                 0);

    if (n <= 0)
    {
        printf("Server disconnected.\n");
        close(sock);
        return 0;
    }

    buffer[n] = '\0';

    if (strcmp(buffer, "FILE_NOT_FOUND") == 0)
    {
        printf("File not found on server.\n");
        close(sock);
        return 0;
    }

    if (strcmp(buffer, "FILE_FOUND") == 0)
    {
        printf("File found.\n");
        printf("Downloading...\n");
    }

    FILE *fp = fopen(filename, "wb");

    if (fp == NULL)
    {
        perror("File creation failed");
        close(sock);
        return 1;
    }

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        n = recv(sock,
                 buffer,
                 sizeof(buffer),
                 0);

        if (n <= 0)
            break;

        fwrite(buffer, 1, n, fp);
    }

    fclose(fp);

    printf("File downloaded successfully.\n");

    close(sock);

    return 0;
}