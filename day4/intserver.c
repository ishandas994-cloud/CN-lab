#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int isPrime(int n)
{
    int i;

    if (n <= 1)
        return 0;

    for (i = 2; i * i <= n; i++)
    {
        if (n % i == 0)
            return 0;
    }

    return 1;
}

int isPalindrome(int n)
{
    int original;
    int reverse = 0;
    int remainder;

    if (n < 0)
        return 0;

    original = n;

    while (n != 0)
    {
        remainder = n % 10;
        reverse = reverse * 10 + remainder;
        n = n / 10;
    }

    return original == reverse;
}

int reverseNumber(int n)
{
    int reverse = 0;
    int remainder;

    if (n < 0)
        return 0;

    while (n != 0)
    {
        remainder = n % 10;
        reverse = reverse * 10 + remainder;
        n = n / 10;
    }

    return reverse;
}

char asciiValue(int n)
{
    return (char)n;
}
struct Request
{
    int number;
    int choice;
};

int main()
{
    int server_port = 5020;

    int sockfd;
    int receive;
    int send;

    char buffer[1024];

    struct Request request;

    struct sockaddr_in server;
    struct sockaddr_in client;

    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("Error in socket()\n");
        return 1;
    }

    printf("Socket created successfully: %d\n", sockfd);

    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(
            sockfd,
            (struct sockaddr *)&server,
            sizeof(server)) == -1)
    {
        printf("Error in bind()\n");
        return 1;
    }

    printf("Bind successful\n");
    printf("Server started...\n");

    len = sizeof(client);

    while (1)
    {
        receive = recvfrom(
            sockfd,
            &request,
            sizeof(request),
            0,
            (struct sockaddr *)&client,
            &len
        );

        if (receive == -1)
        {
            printf("Error in recvfrom()\n");
            return 1;
        }

        printf("\nReceived number: %d\n", request.number);
        printf("Received choice: %d\n", request.choice);
        if (request.choice == 0)
        {
            printf("Client requested exit.\n");
            break;
        }

        switch (request.choice)
        {
            case 1:

                if (isPrime(request.number))
                {
                    snprintf(
                        buffer,
                        sizeof(buffer),
                        "%d is a Prime number",
                        request.number
                    );
                }
                else
                {
                    snprintf(
                        buffer,
                        sizeof(buffer),
                        "%d is not a Prime number",
                        request.number
                    );
                }

                break;
            case 2:

                if (isPalindrome(request.number))
                {
                    snprintf(
                        buffer,
                        sizeof(buffer),
                        "%d is a Palindrome",
                        request.number
                    );
                }
                else
                {
                    snprintf(
                        buffer,
                        sizeof(buffer),
                        "%d is not a Palindrome",
                        request.number
                    );
                }

                break;

            case 3:

                snprintf(
                    buffer,
                    sizeof(buffer),
                    "Reverse of %d is %d",
                    request.number,
                    reverseNumber(request.number)
                );

                break;

            case 4:

                if (request.number >= 0 &&
                    request.number <= 127)
                {
                    snprintf(
                        buffer,
                        sizeof(buffer),
                        "ASCII character of %d is %c",
                        request.number,
                        asciiValue(request.number)
                    );
                }
                else
                {
                    snprintf(
                        buffer,
                        sizeof(buffer),
                        "%d is not a valid ASCII value",
                        request.number
                    );
                }

                break;

            default:

                snprintf(
                    buffer,
                    sizeof(buffer),
                    "Invalid choice"
                );

                break;
        }

        send = sendto(
            sockfd,
            buffer,
            strlen(buffer) + 1,
            0,
            (struct sockaddr *)&client,
            len
        );

        if (send == -1)
        {
            printf("Error in sendto()\n");
            return 1;
        }

        printf("Reply sent: %s\n", buffer);
    }

    return 0;
}