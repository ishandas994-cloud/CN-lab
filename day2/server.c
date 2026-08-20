#include <stdio.h>
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
        printf("error\n");
        return 1;
    }
    else
    {
        printf("socket is fine\n");
    }

    printf("socket value is : %d\n", sockfd);

    char charBuffer[100];

    printf("Enter message: ");
    fgets(charBuffer, sizeof(charBuffer), stdin);

    int rev = sendto(
        sockfd,
        charBuffer,
        sizeof(charBuffer),
        0,
        (struct sockaddr *)&client,
        sizeof(client)
    );

    if (rev == -1)
    {
        printf("Error in sending\n");
        return 1;
    }
    else
    {
        printf("Message sent successfully\n");
    }

    close(sockfd);

    return 0;
}