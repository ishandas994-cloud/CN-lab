#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

struct Request
{
    int number;
    int choice;
};

int main()
{
    int client_port = 5017;
    int server_port = 5020;

    int sockfd;
    int send, receive;

    struct Request request;

    char buffer[1024];

    struct sockaddr_in client, server;
    socklen_t len;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("Error in socket()\n");
        return 1;
    }

    printf("Socket created successfully: %d\n", sockfd);

    client.sin_family = AF_INET;
    client.sin_port = htons(client_port);
    client.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(sockfd, (struct sockaddr *)&client, sizeof(client)) == -1)
    {
        printf("Error in bind()\n");
        return 1;
    }
    printf("Bind successful\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(server);

    while (1)
    {
        printf("\n========== MENU ==========\n");
        printf("1. Check Prime\n");
        printf("2. Check Palindrome\n");
        printf("3. reverse of the input no \n");
        printf("4.find ascii value\n");
        printf("0. Exit\n");
        printf("==========================\n");

        printf("Enter your choice: ");
        scanf("%d", &request.choice);

        if (request.choice == 0)
        {
            request.number = 0;

            send = sendto(
                sockfd,
                &request,
                sizeof(request),
                0,
                (struct sockaddr *)&server,
                sizeof(server)
            );

            if (send == -1)
            {
                printf("Error in sending exit request\n");
                return 1;
            }

            printf("Client exiting...\n");
            break;
        }
        if (request.choice < 1 || request.choice > 4)
        {
            printf("Invalid choice. Try again.\n");
            continue;
        }

        printf("Enter an integer: ");
        scanf("%d", &request.number);
        send = sendto(
            sockfd,
            &request,
            sizeof(request),
            0,
            (struct sockaddr *)&server,
            sizeof(server)
        );

        if (send == -1)
        {
            printf("Error in sending request\n");
            return 1;
        }

        printf("Request sent to server.\n");
        receive = recvfrom(
            sockfd,
            buffer,
            sizeof(buffer) - 1,
            0,
            (struct sockaddr *)&server,
            &len
        );

        if (receive == -1)
        {
            printf("Error in recvfrom()\n");
            return 1;
        }

        buffer[receive] = '\0';

        printf("\n========== SERVER REPLY ==========\n");
        printf("%s\n", buffer);
        printf("==================================\n");
    }

    return 0;
}