#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SEGMENT_SIZE 10

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    char buffer[SEGMENT_SIZE + 1];
    char full_message[1000] = "";

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(server_fd, 2);

    printf("Server waiting for client...\n");

    client_fd = accept(
        server_fd,
        (struct sockaddr*)&client_addr,
        &addr_len
    );

    printf("Client connected.\n\n");

    while (1) {
        memset(buffer, 0, sizeof(buffer));

        int bytes_received = recv(
            client_fd,
            buffer,
            SEGMENT_SIZE,
            0
        );

        if (bytes_received <= 0)
            break;

        buffer[bytes_received] = '\0';

        printf("Received Segment: %s\n", buffer);

        strcat(full_message, buffer);

        char ack[] = "ACK";
        send(client_fd, ack, strlen(ack), 0);
    }

    printf("\n--------------------------------\n");
    printf("Complete Message:\n");
    printf("%s\n", full_message);
    printf("--------------------------------\n");

    close(client_fd);
    close(server_fd);

    return 0;
}
