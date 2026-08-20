#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#define MAX 1024

int main()
{
    int client_port = 5017;
    int server_port = 5020;

    int sockfd, b, send, receive;
    char charBuffer[MAX];

    struct sockaddr_in client, server;
    socklen_t len;

    fd_set readfds;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("Error in socket()\n");
        return 1;
    }

    printf("sockfd() success: %d\n", sockfd);

    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    b = bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    if (b == -1)
    {
        printf("Error in bind()\n");
        return 1;
    }

    printf("Bind successful\n");
    printf("Server started\n");

    len = sizeof(client);

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        FD_SET(STDIN_FILENO, &readfds);

        if (select(sockfd + 1, &readfds, NULL, NULL, NULL) == -1)
        {
            printf("Error in select()\n");
            return 1;
        }

        if (FD_ISSET(sockfd, &readfds))
        {
            receive = recvfrom(
                sockfd,
                charBuffer,
                MAX - 1,
                0,
                (struct sockaddr *)&client,
                &len);

            if (receive == -1)
            {
                printf("Error in recvfrom()\n");
                return 1;
            }

            charBuffer[receive] = '\0';

            printf("\nClient: %s\n", charBuffer);

            if (strcmp(charBuffer, "quit") == 0 ||
                strcmp(charBuffer, "close") == 0)
            {
                printf("Client closed the chat\n");
                break;
            }
        }

        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            printf("Server: ");

            fgets(charBuffer, MAX, stdin);
            charBuffer[strcspn(charBuffer, "\n")] = '\0';

            send = sendto(
                sockfd,
                charBuffer,
                strlen(charBuffer),
                0,
                (struct sockaddr *)&client,
                len);

            if (send == -1)
            {
                printf("Error in sendto()\n");
                return 1;
            }

            if (strcmp(charBuffer, "quit") == 0 ||
                strcmp(charBuffer, "close") == 0)
            {
                printf("Chat closed\n");
                break;
            }
        }
    }

    close(sockfd);

    return 0;
}