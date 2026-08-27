#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    struct sockaddr_in server = {
        .sin_family = AF_INET,
        .sin_port = htons(8888),
        .sin_addr.s_addr = inet_addr("127.0.0.1")
    };

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1) {
        printf("Socket error\n");
        return 1;
    }

    char buffer[100];
    char result[200];

    printf("Enter message: ");
    fgets(buffer, sizeof(buffer), stdin);


    buffer[strcspn(buffer, "\n")] = '\0';

   
    if (sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server, sizeof(server)) == -1) {
        printf("Sending error\n");
        return 1;
    }

    int n = recvfrom(sockfd, result, sizeof(result) - 1, 0,
                     NULL, NULL);

    if (n == -1) {
        printf("Receive error\n");
        return 1;
    }

    result[n] = '\0';

    printf("\n----- CLIENT -----\n");
    printf("Message: %s\n", buffer);
    printf("%s\n", result);

    close(sockfd);
    return 0;
}