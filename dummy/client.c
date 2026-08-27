#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
    struct sockaddr_in server = {
        .sin_family = AF_INET,
        .sin_port = htons(8889),
        .sin_addr.s_addr = inet_addr("127.0.0.1")
    };

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1) {
        printf("Socket error\n");
        return 1;
    }

    char buffer[100];
    struct sockaddr_in from;
    socklen_t len = sizeof(server);

    while (1)
    {
        printf("\n1. Prime\n");
        printf("2. Palindrome\n");
        printf("3. ASCII\n");
        printf("4. Reverse\n");
        printf("5. Binary\n");
        printf("6. Quit\n");

        printf("Enter choice: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        if (strcmp(buffer, "6") == 0)
        {
            sendto(sockfd, "quit", 4, 0,
                   (struct sockaddr *)&server, sizeof(server));
            break;
        }

        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr *)&server, sizeof(server));

        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&from, &len);

        if (n == -1) {
            printf("Receive error\n");
            break;
        }

        buffer[n] = '\0';
        printf("%s", buffer);

        if (strcmp(buffer, "Enter number: ") == 0)
        {
            printf("\n");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';

            sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&server, sizeof(server));
        }
        else if (strcmp(buffer, "Enter character: ") == 0)
        {
            printf("\n");
            fgets(buffer, sizeof(buffer), stdin);
            buffer[strcspn(buffer, "\n")] = '\0';

            sendto(sockfd, buffer, strlen(buffer), 0,
                   (struct sockaddr *)&server, sizeof(server));
        }
        else
        {
            continue;
        }

        n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&from, &len);

        if (n == -1) {
            printf("Receive error\n");
            break;
        }

        buffer[n] = '\0';
        printf("Result: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}