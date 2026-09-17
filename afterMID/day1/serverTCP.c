#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int server_fd, client_fd;
    char buffer[1024];

    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd == -1)
    {
        printf("Socket error\n");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) == -1)
    {
        perror("Bind failed");
        return 1;
    }

    listen(server_fd, 2);

    printf("Server listening...\n");

    // Client 1
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                       &addr_size);

    if (client_fd == -1)
    {
        perror("Accept failed");
        return 1;
    }

    memset(buffer, 0, sizeof(buffer));

    recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    printf("From Client 1: %s", buffer);

    close(client_fd);

    // Client 2
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                       &addr_size);

    if (client_fd == -1)
    {
        perror("Accept failed");
        return 1;
    }

    memset(buffer, 0, sizeof(buffer));

    recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    printf("From Client 2: %s", buffer);

    close(client_fd);

    close(server_fd);

    return 0;
}