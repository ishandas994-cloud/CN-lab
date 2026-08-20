#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main()
{
    int sockfd;
    char charBuffer[100];

    struct sockaddr_in server, client;
    socklen_t len;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("Error in socket()\n");
        return 1;
    }

    printf("Socket created successfully\n");
    printf("Socket value is: %d\n", sockfd);

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Bind socket
    int b = bind(
        sockfd,
        (struct sockaddr *)&server,
        sizeof(server)
    );

    if (b == -1)
    {
        printf("Error in bind()\n");
        close(sockfd);
        return 1;
    }

    printf("Bind successful\n");
    printf("Server started...\n");

    len = sizeof(client);

    while (1)
    {
        int receive = recvfrom(
            sockfd,
            charBuffer,
            sizeof(charBuffer) - 1,
            0,
            (struct sockaddr *)&client,
            &len
        );

        if (receive == -1)
        {
            printf("Error in receiving\n");
            break;
        }

        charBuffer[receive] = '\0';

        printf("Message received: %s\n", charBuffer);

        if (strcmp(charBuffer, "quit") == 0)
        {
            printf("Quit received. Server shutting down...\n");
            break;
        }
    }

    close(sockfd);

    return 0;
}