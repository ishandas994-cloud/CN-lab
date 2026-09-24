#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 5

int main()
{
    int server_fd, client_fd;
    int clients[MAX_CLIENTS];
    int count = 0;

    char buffer[1024];

    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    fd_set readfds;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (server_fd < 0)
    {
        perror("Socket failed");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, MAX_CLIENTS) < 0)
    {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    printf("Server started...\n");
    printf("Waiting for clients...\n");

    while (1)
    {
        FD_ZERO(&readfds);

        FD_SET(server_fd, &readfds);

        int max_fd = server_fd;

        if (count > 0)
        {
            FD_SET(clients[0], &readfds);

            if (clients[0] > max_fd)
                max_fd = clients[0];
        }

        FD_SET(STDIN_FILENO, &readfds);

        if (STDIN_FILENO > max_fd)
            max_fd = STDIN_FILENO;

        select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(server_fd, &readfds))
        {
            client_fd = accept(server_fd,
                               (struct sockaddr *)&client_addr,
                               &addr_size);

            if (client_fd < 0)
            {
                perror("Accept failed");
                continue;
            }

            if (count >= MAX_CLIENTS)
            {
                send(client_fd,
                     "Server full\n",
                     strlen("Server full\n"),
                     0);

                close(client_fd);
                continue;
            }

            clients[count] = client_fd;

            printf("\nClient %d connected.\n", count + 1);

            if (count == 0)
            {
                send(client_fd,
                     "You are now active. You can chat with server.\n",
                     strlen("You are now active. You can chat with server.\n"),
                     0);

                printf("Client 1 is now active.\n");
            }
            else
            {
                send(client_fd,
                     "WAIT: You are in the queue.\n",
                     strlen("WAIT: You are in the queue.\n"),
                     0);

                printf("Client %d is waiting.\n", count + 1);
            }

            count++;
        }

        if (count > 0 && FD_ISSET(clients[0], &readfds))
        {
            memset(buffer, 0, sizeof(buffer));

            int n = recv(clients[0],
                         buffer,
                         sizeof(buffer) - 1,
                         0);

            if (n <= 0)
            {
                printf("Client 1 terminated.\n");

                close(clients[0]);

                for (int i = 0; i < count - 1; i++)
                    clients[i] = clients[i + 1];

                count--;

                if (count > 0)
                {
                    send(clients[0],
                         "Your turn. You can chat now.\n",
                         strlen("Your turn. You can chat now.\n"),
                         0);

                    printf("Client 2 is now active.\n");
                }
            }
            else
            {
                buffer[n] = '\0';

                printf("\nClient 1: %s\n", buffer);

                if (strcmp(buffer, "bye") == 0)
                {
                    send(clients[0],
                         "Goodbye. Connection closed.\n",
                         strlen("Goodbye. Connection closed.\n"),
                         0);

                    printf("Client 1 terminated.\n");

                    close(clients[0]);

                    for (int i = 0; i < count - 1; i++)
                        clients[i] = clients[i + 1];

                    count--;

                    if (count > 0)
                    {
                        send(clients[0],
                             "Your turn. You can chat now.\n",
                             strlen("Your turn. You can chat now.\n"),
                             0);

                        printf("Next client is now active.\n");
                    }
                }
                else
                {
                    printf("Server: ");

                    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
                    {
                        buffer[strcspn(buffer, "\n")] = '\0';

                        send(clients[0],
                             buffer,
                             strlen(buffer),
                             0);
                    }
                }
            }
        }

        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            if (count > 0)
            {
                printf("Server: ");

                if (fgets(buffer, sizeof(buffer), stdin) != NULL)
                {
                    buffer[strcspn(buffer, "\n")] = '\0';

                    if (strlen(buffer) > 0)
                    {
                        send(clients[0],
                             buffer,
                             strlen(buffer),
                             0);
                    }
                }
            }
        }
    }

    close(server_fd);

    return 0;
}