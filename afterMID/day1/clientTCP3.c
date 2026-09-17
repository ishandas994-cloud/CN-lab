#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int sockfd;
    char message[1024];

    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        printf("Socket error\n");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) == -1)
    {
        perror("Connection failed");
        return 1;
    }

    printf("Connected to server\n");

    printf("Enter message: ");
    fgets(message, sizeof(message), stdin);

    send(sockfd, message, strlen(message), 0);

    close(sockfd);

    return 0;
}