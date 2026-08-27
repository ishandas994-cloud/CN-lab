#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

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

    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1) {
        printf("Bind error\n");
        return 1;
    }

    char buffer[100];
    char result[250];

    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                     (struct sockaddr *)&client, &len);

    if (n == -1) {
        printf("Receive error\n");
        return 1;
    }

    buffer[n] = '\0';

    int vowels = 0;
    int consonants = 0;
    int digits = 0;
    int spaces = 0;
    int special = 0;

    for (int i = 0; buffer[i] != '\0'; i++)
    {
        char ch = buffer[i];

        if (isalpha(ch))
        {
            if (ch == 'a' || ch == 'e' || ch == 'i' ||
                ch == 'o' || ch == 'u' ||
                ch == 'A' || ch == 'E' || ch == 'I' ||
                ch == 'O' || ch == 'U')
                vowels++;
            else
                consonants++;
        }
        else if (isdigit(ch))
            digits++;
        else if (isspace(ch))
            spaces++;
        else
            special++;
    }

    int palindrome = 1;
    int left = 0;
    int right = strlen(buffer) - 1;

    while (left < right)
    {
        if (buffer[left] != buffer[right])
        {
            palindrome = 0;
            break;
        }

        left++;
        right--;
    }

    printf("\n-- SERVER --\n");
    printf("Message: %s\n", buffer);
    printf("No of Vowels       : %d\n", vowels);
    printf("No of Consonants   : %d\n", consonants);
    printf("No of Digits       : %d\n", digits);
    printf("No of Spaces       : %d\n", spaces);
    printf("No of Special Char : %d\n", special);

    if (palindrome)
        printf("Is the string is Palindrome   : Yes\n");
    else
        printf("Is the string Palindrome   : No\n");

    sprintf(result,
            "Vowels       : %d\n"
            "Consonants   : %d\n"
            "Digits       : %d\n"
            "Spaces       : %d\n"
            "Special Char : %d\n"
            "Palindrome   : %s",
            vowels,
            consonants,
            digits,
            spaces,
            special,
            palindrome ? "Yes" : "No");

    sendto(sockfd, result, strlen(result), 0,
           (struct sockaddr *)&client, len);

    close(sockfd);
    return 0;
}