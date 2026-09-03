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
    char name[100];
    int roll, m1, m2, m3;

    struct sockaddr_in client, server;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("Error in socket()\n");
        return 1;
    }

    printf("sockfd() success: %d\n", sockfd);

    client.sin_family = AF_INET;
    client.sin_port = htons(client_port);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    b = bind(sockfd, (struct sockaddr *)&client, sizeof(client));

    if (b == -1)
    {
        printf("Error in bind()\n");
        return 1;
    }

    printf("Bind successful\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(server);


    printf("\nEnter Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter Roll No: ");
    scanf("%d", &roll);

    printf("Enter Marks of Subject 1: ");
    scanf("%d", &m1);

    printf("Enter Marks of Subject 2: ");
    scanf("%d", &m2);

    printf("Enter Marks of Subject 3: ");
    scanf("%d", &m3);

   
    sprintf(charBuffer, "%s,%d,%d,%d,%d",
            name, roll, m1, m2, m3);

   
    send = sendto(
        sockfd,
        charBuffer,
        strlen(charBuffer),
        0,
        (struct sockaddr *)&server,
        len);

    if (send == -1)
    {
        printf("Error in sendto()\n");
        return 1;
    }

    printf("\nStudent details sent to server\n");

    receive = recvfrom(
        sockfd,
        charBuffer,
        MAX - 1,
        0,
        (struct sockaddr *)&server,
        &len);

    if (receive == -1)
    {
        printf("Error in recvfrom()\n");
        return 1;
    }

    charBuffer[receive] = '\0';

    printf("\n------ RESULT FROM SERVER ------\n");
    printf("%s\n", charBuffer);

    close(sockfd);

    return 0;
}