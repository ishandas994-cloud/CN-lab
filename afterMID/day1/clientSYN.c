#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define SEGMENT_SIZE 10

int main() {
    int sock;
    struct sockaddr_in server_addr;

    char message[1000];
    char buffer[SEGMENT_SIZE + 1];
    char ack[10];

    printf("Enter long message: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = '\0';

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    connect(
        sock,
        (struct sockaddr*)&server_addr,
        sizeof(server_addr)
    );

    int length = strlen(message);
    int position = 0;
    int segment_number = 1;

    while (position < length) {

        int remaining = length - position;

        int bytes_to_send;

        if (remaining >= SEGMENT_SIZE)
            bytes_to_send = SEGMENT_SIZE;
        else
            bytes_to_send = remaining;

        memset(buffer, 0, sizeof(buffer));

        memcpy(
            buffer,
            message + position,
            bytes_to_send
        );

        buffer[bytes_to_send] = '\0';

        printf("\nSending Segment %d: %s\n",
               segment_number,
               buffer);

        send(
            sock,
            buffer,
            bytes_to_send,
            0
        );

        memset(ack, 0, sizeof(ack));

        recv(
            sock,
            ack,
            sizeof(ack) - 1,
            0
        );

        printf("Server ACK: %s\n", ack);

        position += bytes_to_send;
        segment_number++;
    }

    close(sock);

    return 0;
}
