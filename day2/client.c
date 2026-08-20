#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <netinet/ip.h> 
#include <unistd.h> 
 
int main() 
{ 
    struct sockaddr_in client; 
 
    client.sin_family = AF_INET; 
    client.sin_port = htons(8888); 
    client.sin_addr.s_addr = inet_addr("127.0.0.1"); 
 
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0); 
 
    if (sockfd == -1) 
    { 
        printf("error\n"); 
        return 1; 
    } 
    else 
    { 
        printf("socket is fine\n"); 
    } 
 
    printf("socket value is : %d\n", sockfd); 
 
    int b = bind(sockfd, (struct sockaddr *)&client, sizeof(client)); 
 
    if (b == -1) 
    { 
        printf("Error in bind\n");        
        return 1; 
    } 
    else 
    { 
        printf("Client bind successful: %d\n", b); 
    } 
 
    char charBuffer[100]; 
 
    int flag = 0; 
 
    struct sockaddr_in client_addr; 
    socklen_t temp = sizeof(client_addr); 
 
    int rev = recvfrom( 
        sockfd, 
        charBuffer, 
        sizeof(charBuffer) - 1, 
        flag, 
        (struct sockaddr *)&client_addr, 
        &temp 
    ); 
 
    if (rev == -1) 
    { 
        printf("Error at server side\n"); 
        return 1; 
    } 
    else 
    { 
        charBuffer[rev] = '\0'; 
        printf("Message is received: %s\n", charBuffer);       
        printf("Message is received: %s\n", charBuffer); 
    } 
 
    close(sockfd); 
 
    return 0; 
}