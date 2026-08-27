#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    struct sockaddr_in server = {
        .sin_family = AF_INET,
        .sin_port = htons(8889),
        .sin_addr.s_addr = INADDR_ANY
    };

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1) {
        printf("Socket error\n");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        printf("Bind error\n");
        return 1;
    }

    char buffer[100], result[100];
    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    while (1)
    {
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&client, &len);

        if (n == -1) {
            printf("Receive error\n");
            break;
        }

        buffer[n] = '\0';

        if (strcmp(buffer, "quit") == 0)
            break;

        int choice = atoi(buffer);
        int x, z = 0;

        if (choice == 1 || choice == 2 || choice == 4 || choice == 5)
        {
            sendto(sockfd, "Enter number: ", 14, 0,
                   (struct sockaddr *)&client, len);

            n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&client, &len);

            buffer[n] = '\0';
            x = atoi(buffer);
        }

        if (choice == 1)
        {
            if (x < 2)
                z = 1;

            for (int i = 2; i * i <= x; i++)
            {
                if (x % i == 0)
                {
                    z = 1;
                    break;
                }
            }

            sprintf(result, z ? "Not Prime" : "Prime");
        }
        else if (choice == 2)
        {
            int original = x;

            while (x)
            {
                z = z * 10 + x % 10;
                x /= 10;
            }

            sprintf(result, original == z ? "Palindrome" : "Not Palindrome");
        }
        else if (choice == 3)
        {
            sendto(sockfd, "Enter character: ", 17, 0,
                   (struct sockaddr *)&client, len);

            n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&client, &len);

            buffer[n] = '\0';

            sprintf(result, "ASCII value: %d", buffer[0]);
        }
        else if (choice == 4)
        {
            while (x)
            {
                z = z * 10 + x % 10;
                x /= 10;
            }

            sprintf(result, "Reverse: %d", z);
        }
        else if (choice == 5)
        {
            int binary[32];
            int i = 0;
            int num = x;

            if (num == 0)
            {
                sprintf(result, "Binary: 0");
            }
            else
            {
                while (num > 0)
                {
                    binary[i++] = num % 2;
                    num /= 2;
                }

                int pos = 0;
                pos += sprintf(result + pos, "Binary: ");

                while (i > 0)
                    pos += sprintf(result + pos, "%d", binary[--i]);
            }
        }
        else
        {
            sprintf(result, "Invalid choice");
        }

        sendto(sockfd, result, strlen(result), 0,
               (struct sockaddr *)&client, len);
    }

    close(sockfd);
    return 0;
}