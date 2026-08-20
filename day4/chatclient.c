#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

int main()
{
    struct sockaddr_in client;

    client.sin_family = AF_INET;
    client.sin_port = htons(8888);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("Error in socket()\n");
        return 1;
    }

    printf("Socket is fine\n");
    printf("Socket value is: %d\n", sockfd);

    char charBuffer[100];

    while (1)
    {
        printf("Enter message: ");

        fgets(charBuffer, sizeof(charBuffer), stdin);

       
        charBuffer[strcspn(charBuffer, "\n")] = '\0';

        
        if (strcmp(charBuffer, "quit") == 0)
        {
            printf("Quitting...\n");
            break;
        }

        int rev = sendto(
            sockfd,
            charBuffer,
            strlen(charBuffer),
            0,
            (struct sockaddr *)&client,
            sizeof(client)
        );

        if (rev == -1)
        {
            printf("Error in sending\n");
            break;
        }
        else
        {
            printf("Message sent successfully\n");
        }
    }

    close(sockfd);

    return 0;
}