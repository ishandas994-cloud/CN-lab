#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX 1024

int main()
{
    int server_port = 5020;
    int sockfd, b, send, receive;

    char charBuffer[MAX];
    char name[100];
    int roll, m1, m2, m3;
    int total;
    float average;

    struct sockaddr_in client, server;
    socklen_t len;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd == -1)
    {
        printf("Error in socket()\n");
        return 1;
    }

    printf("sockfd() success: %d\n", sockfd);

    server.sin_family = AF_INET;
    server.sin_port = htons(server_port);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    b = bind(sockfd, (struct sockaddr *)&server, sizeof(server));

    if (b == -1)
    {
        printf("Error in bind()\n");
        return 1;
    }

    printf("Bind successful\n");
    printf("Server started\n");

    len = sizeof(client);

    while (1)
    {
        receive = recvfrom(
            sockfd,
            charBuffer,
            MAX - 1,
            0,
            (struct sockaddr *)&client,
            &len);

        if (receive == -1)
        {
            printf("Error in recvfrom()\n");
            return 1;
        }

        charBuffer[receive] = '\0';

        sscanf(charBuffer, "%[^,],%d,%d,%d,%d",
               name, &roll, &m1, &m2, &m3);

        printf("\nStudent Details Received:\n");
        printf("Name: %s\n", name);
        printf("Roll No: %d\n", roll);
        printf("Marks: %d %d %d\n", m1, m2, m3);

        total = m1 + m2 + m3;
        average = total / 3.0;

        if (m1 < 40 || m2 < 40 || m3 < 40)
        {
            sprintf(charBuffer,
                    "Name: %s\nRoll: %d\nTotal: %d\nAverage: %.2f\nResult: FAIL",
                    name, roll, total, average);
        }
        else
        {
            if (average >= 90)
            {
                sprintf(charBuffer,
                        "Name: %s\nRoll: %d\nTotal: %d\nAverage: %.2f\nGrade: A+\nResult: PASS",
                        name, roll, total, average);
            }
            else if (average >= 80)
            {
                sprintf(charBuffer,
                        "Name: %s\nRoll: %d\nTotal: %d\nAverage: %.2f\nGrade: A\nResult: PASS",
                        name, roll, total, average);
            }
            else if (average >= 70)
            {
                sprintf(charBuffer,
                        "Name: %s\nRoll: %d\nTotal: %d\nAverage: %.2f\nGrade: B\nResult: PASS",
                        name, roll, total, average);
            }
            else if (average >= 60)
            {
                sprintf(charBuffer,
                        "Name: %s\nRoll: %d\nTotal: %d\nAverage: %.2f\nGrade: C\nResult: PASS",
                        name, roll, total, average);
            }
            else
            {
                sprintf(charBuffer,
                        "Name: %s\nRoll: %d\nTotal: %d\nAverage: %.2f\nGrade: D\nResult: PASS",
                        name, roll, total, average);
            }
        }

        send = sendto(
            sockfd,
            charBuffer,
            strlen(charBuffer),
            0,
            (struct sockaddr *)&client,
            len);

        if (send == -1)
        {
            printf("Error in sendto()\n");
            return 1;
        }

        printf("\nResult sent to client\n");
    }

    close(sockfd);

    return 0;
}