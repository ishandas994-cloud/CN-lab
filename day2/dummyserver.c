#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
int main(){
    struct sockaddr_in client;

    client.sin_family=AF_INET;
    client.sin_port=htons(8888);
    client.sin_addr.s_addr=inet_addr("127.0.0.1");

    int socketfd=socket(AF_INET,SOCK_DGRAM,0);
    if(socketfd==-1){
        printf("some errors are coming:");
        return 1;
    }
    else{
        printf("fine:\n");
    }
    printf("sock value %d :",socketfd);
    int b =bind(socketfd,(struct sockaddr *)&client,sizeof(client));
    if(b==-1){
        printf("error");
    }else{
        printf("bind value is : %d",b);
    }
}