#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(){
    struct sockaddr_in client;

    client.sin_family = AF_INET;
    client.sin_port=htons(8888);
    client.sin_addr.s_addr=inet_addr("127.0.0.1");

    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd==-1){
        printf("something wrong\n");
        return 1;
    }
    else{
        printf("socket is fine:");

    }
    printf("socket value is : %d\n",sockfd);
return 0;
}