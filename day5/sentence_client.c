#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
    memset(&client, 0, sizeof(client));
    memset(&server, 0, sizeof(server));

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        perror("Error in socket");
        return 1;
    }

    printf("Socket created successfully\n");

    client.sin_family = AF_INET;
    client.sin_port = htons(client_port);
    client.sin_addr.s_addr = INADDR_ANY;
    b = bind(sockfd, (struct sockaddr *)&client, sizeof(client));

    if (b == -1)
    {
        perror("Error in bind");
        return 1;
    }

    printf("Client bind successful\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(server);
    printf("\nEnter a sentence: ");

    fgets(charBuffer, MAX, stdin);

    charBuffer[strcspn(charBuffer, "\n")] = '\0';

    send = sendto(
        sockfd,
        charBuffer,
        strlen(charBuffer),
        0,
        (struct sockaddr *)&server,
        len
    );

    if (send == -1)
    {
        perror("Error in sendto");
        return 1;
    }

    printf("Sentence sent to server\n");

    receive = recvfrom(
        sockfd,
        charBuffer,
        MAX - 1,
        0,
        (struct sockaddr *)&server,
        &len
    );

    if (receive == -1)
    {
        perror("Error in recvfrom");
        return 1;
    }

    charBuffer[receive] = '\0';
    printf("\n------ RESULT FROM SERVER ------\n");
    printf("%s\n", charBuffer);


    close(sockfd);

    return 0;
}