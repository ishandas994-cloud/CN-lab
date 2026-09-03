#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX 1024

void findLongestWord(char sentence[], char longest[])
{
    char temp[MAX];
    char *word;

    strcpy(temp, sentence);

    longest[0] = '\0';

    word = strtok(temp, " ");

    while (word != NULL)
    {
        if (strlen(word) > strlen(longest))
        {
            strcpy(longest, word);
        }

        word = strtok(NULL, " ");
    }
}

void findEvenWords(char sentence[], char evenWords[])
{
    char temp[MAX];
    char *word;

    strcpy(temp, sentence);

    evenWords[0] = '\0';

    word = strtok(temp, " ");

    while (word != NULL)
    {
        if (strlen(word) % 2 == 0)
        {
            strcat(evenWords, word);
            strcat(evenWords, " ");
        }

        word = strtok(NULL, " ");
    }
}


int main()
{
    int server_port = 5020;

    int sockfd, b, send, receive;

    char charBuffer[MAX];
    char longest[100];
    char evenWords[MAX];
    char result[MAX];

    struct sockaddr_in client, server;
    socklen_t len;

    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        perror("Error in socket");
        return 1;
    }

    printf("Socket created successfully\n");

    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = INADDR_ANY;

    b = bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    if (b == -1)
    {
        perror("Error in bind");
        return 1;
    }

    printf("Bind successful\n");
    printf("Server started on port %d\n", server_port);

    len = sizeof(client);


    while (1)
    {
        receive = recvfrom(
            sockfd,
            charBuffer,
            MAX - 1,
            0,
            (struct sockaddr *)&client,
            &len
        );

        if (receive == -1)
        {
            perror("Error in recvfrom");
            return 1;
        }

        charBuffer[receive] = '\0';

        printf("\nSentence received from client:\n");
        printf("%s\n", charBuffer);

        findLongestWord(charBuffer, longest);
        findEvenWords(charBuffer, evenWords);
        sprintf(result,
                "Longest Word: %s\nEven Length Words: %s",
                longest, evenWords);

        send = sendto(
            sockfd,
            result,
            strlen(result),
            0,
            (struct sockaddr *)&client,
            len
        );

        if (send == -1)
        {
            perror("Error in sendto");
            return 1;
        }

        printf("\nResult sent to client\n");
    }

    close(sockfd);

    return 0;
}