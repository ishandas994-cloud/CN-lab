#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>

void checkString(char str[])
{
    int vowels = 0, consonants = 0;
    int digits = 0, spaces = 0, special = 0;

    for (int i = 0; str[i] != '\0'; i++)
    {
        char ch = str[i];

        if (isalpha(ch))
        {
            if (ch == 'a' || ch == 'e' || ch == 'i' ||
                ch == 'o' || ch == 'u' ||
                ch == 'A' || ch == 'E' || ch == 'I' ||
                ch == 'O' || ch == 'U')
            {
                vowels++;
            }
            else
            {
                consonants++;
            }
        }
        else if (isdigit(ch))
        {
            digits++;
        }
        else if (isspace(ch))
        {
            spaces++;
        }
        else
        {
            special++;
        }
    }

    printf("Vowels      : %d\n", vowels);
    printf("Consonants  : %d\n", consonants);
    printf("Digits      : %d\n", digits);
    printf("Spaces      : %d\n", spaces);
    printf("Special Char: %d\n", special);
}

int main()
{
    struct sockaddr_in server = {
        .sin_family = AF_INET,
        .sin_port = htons(8888),
        .sin_addr.s_addr = inet_addr("127.0.0.1")
    };

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("Socket error\n");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("Bind error\n");
        return 1;
    }

    char buffer[100];
    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&client, &len);

    if (n == -1)
    {
        printf("Receive error\n");
        return 1;
    }

    buffer[n] = '\0';

    printf("Message received: %s\n", buffer);

    checkString(buffer);

    close(sockfd);
    return 0;
}