#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#define PORT 12345

int main() {
    int sockFD;
    struct sockaddr_in servaddr;
    sockFD = socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof(servaddr));
    if(sockFD < 0) {
        printf("Error....\n");
        exit(0);
    }
    else {
        printf("Socket is created\n");
    }
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(PORT);
    if(connect(sockFD,(struct sockaddr *)&servaddr,sizeof(servaddr))<0) {
        printf("Error in connection\n");
        exit(0);
    }
    else {
        printf("connected\n");
    }
    char buffer[50] = "";
    while(1) {
        bzero(buffer, sizeof(buffer));
        printf("\n$\t:\t");
        scanf("%[^\n]%*c" , buffer);
        write(sockFD , buffer, strlen(buffer));
        printf("\nMessage Sent\t:\t%s\n" , buffer);
        if(strcmp(buffer ,"QUIT") == 0 ) {
            break;
        }
        read(sockFD , buffer , sizeof(buffer));
        printf("\nMessage Received\t:\t%s\n" , buffer);
    }
    close(sockFD);

    return 0;
}
