#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <dirent.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    char buffer[BUFFER_SIZE];
    char filename[256];

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    int port = atoi(argv[1]);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("Socket failed");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    listen(server_fd, 5);

    printf("Server started on port %d...\n", port);
    printf("Waiting for client...\n");

    client_fd = accept(server_fd,
                       (struct sockaddr *)&client_addr,
                       &addr_size);

    if (client_fd < 0)
    {
        perror("Accept failed");
        close(server_fd);
        return 1;
    }

    printf("Client connected.\n");

    DIR *dir;
    struct dirent *entry;

    dir = opendir(".");

    if (dir == NULL)
    {
        perror("Directory error");
        close(client_fd);
        close(server_fd);
        return 1;
    }

    strcpy(buffer, "Files available on server:\n");

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            strcat(buffer, entry->d_name);
            strcat(buffer, "\n");
        }
    }

    closedir(dir);

    strcat(buffer, "END_LIST\n");

    send(client_fd, buffer, strlen(buffer), 0);

    printf("File list sent to client.\n");

    memset(filename, 0, sizeof(filename));

    int n = recv(client_fd,
                 filename,
                 sizeof(filename) - 1,
                 0);

    if (n <= 0)
    {
        printf("Client disconnected.\n");
        close(client_fd);
        close(server_fd);
        return 0;
    }

    filename[n] = '\0';

    printf("Client requested: %s\n", filename);

    FILE *fp = fopen(filename, "rb");

    if (fp == NULL)
    {
        strcpy(buffer, "FILE_NOT_FOUND");
        send(client_fd, buffer, strlen(buffer), 0);

        printf("File not found.\n");

        close(client_fd);
        close(server_fd);

        return 0;
    }

    strcpy(buffer, "FILE_FOUND");

    send(client_fd, buffer, strlen(buffer), 0);

    while ((n = fread(buffer, 1, BUFFER_SIZE, fp)) > 0)
    {
        send(client_fd, buffer, n, 0);
    }

    fclose(fp);

    printf("File sent successfully.\n");

    close(client_fd);

    printf("Client connection closed.\n");

    close(server_fd);

    return 0;
}