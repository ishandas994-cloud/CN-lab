#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main()
{
    int sock;
    char buffer[1024];

    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
    {
        perror("Socket failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1",
                  &server_addr.sin_addr) <= 0)
    {
        perror("Invalid IP");
        close(sock);
        return 1;
    }

    if (connect(sock,
                (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(sock);
        return 1;
    }

    printf("Connected to server.\n");

    while (1)
    {
        memset(buffer, 0, sizeof(buffer));

        int n = recv(sock,
                     buffer,
                     sizeof(buffer) - 1,
                     0);

        if (n <= 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        buffer[n] = '\0';

        printf("%s", buffer);

        if (strstr(buffer, "WAIT") != NULL)
        {
            while (1)
            {
                memset(buffer, 0, sizeof(buffer));

                n = recv(sock,
                         buffer,
                         sizeof(buffer) - 1,
                         0);

                if (n <= 0)
                {
                    printf("Server disconnected.\n");
                    close(sock);
                    return 0;
                }

                buffer[n] = '\0';

                printf("%s", buffer);

                if (strstr(buffer, "Your turn") != NULL)
                    break;
            }
        }

        break;
    }

    while (1)
    {
        printf("You: ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL)
            break;

        buffer[strcspn(buffer, "\n")] = '\0';

        if (strlen(buffer) == 0)
            continue;

        if (send(sock,
                 buffer,
                 strlen(buffer),
                 0) < 0)
        {
            perror("Send failed");
            break;
        }

        if (strcmp(buffer, "bye") == 0)
        {
            memset(buffer, 0, sizeof(buffer));

            int n = recv(sock,
                         buffer,
                         sizeof(buffer) - 1,
                         0);

            if (n > 0)
            {
                buffer[n] = '\0';
                printf("%s", buffer);
            }

            break;
        }

        memset(buffer, 0, sizeof(buffer));

        int n = recv(sock,
                     buffer,
                     sizeof(buffer) - 1,
                     0);

        if (n <= 0)
        {
            printf("Server disconnected.\n");
            break;
        }

        buffer[n] = '\0';

        printf("Server: %s\n", buffer);
    }

    close(sock);

    return 0;
}