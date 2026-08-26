#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<unistd.h>
int main(){
     struct sockaddr_in client;
      client.sin_family=AF_INET;
      client.sin_port=htons(8888);
      client.sin_addr.s_addr=inet_addr("127.0.0.1");

      int sockfd=socket(AF_INET,SOCK_DGRAM,0);

      int b=bind(sockfd,(struct sockaddr *)&client,sizeof(client));
      char charbuffer[100];
      int flag =0;
      struct sockaddr_in client_addr;
      socklen_t temp=sizeof(client_addr);
    int rev=recvfrom(
        sockfd,
        charbuffer,
        sizeof(charbuffer)-1,
        flag,
        (struct sockaddr *)&client_addr,
        &temp
    );
    if(rev==-1){
        printf("error at server side\n");
        return 1;
    }else{
        charbuffer[rev]='\0';
        printf("message is received : %s\n",charbuffer);

    }
    close(sockfd);
    return 0;


}
