#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#define MAX 1024
void reverseString(char str[]);

int palindrom(char str[])
{
    char str2[MAX];

    strcpy(str2, str);
    reverseString(str2);

    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == str2[i])
        {
            continue;
        }
        else
        {
            return 0;
        }
    }

    return 1;
}

void reverseString(char str[])
{
    int i, j;
    char temp;

    j = strlen(str) - 1;

    for (i = 0; i < j; i++, j--)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
    }
}

int main()
{
    int client_port = 5017;
    int server_port = 5020;

    int sockfd, b, send, receive;
    char charBuffer[MAX];

    struct sockaddr_in client, server;
    socklen_t len;

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

    printf("Received from client: %s\n", charBuffer);

    printf("Message length: %lu\n", strlen(charBuffer));

    if (palindrom(charBuffer))
    {
        printf("The message is a palindrome\n");
    }
    else
    {
        printf("The message is not a palindrome\n");
    }

    reverseString(charBuffer);

    printf("Reversed message: %s\n", charBuffer);

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

    return 0;
}