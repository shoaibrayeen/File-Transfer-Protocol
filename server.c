#include <stdio.h>
#include <iostream>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;
#define PORT 12345




int main() {
   int sockFD , bindOutput , acceptOutput , clientLen;
    struct sockaddr_in servaddr,cliaddr;
    sockFD = socket(AF_INET,SOCK_STREAM,0);
    if( sockFD < 0 ) {
        printf("\nSocket Error\n");
        exit(0);
    }
    else {
        printf("\nSocket is created\n");
    }
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(PORT);
    bindOutput = bind(sockFD,(struct sockaddr *)&servaddr, sizeof(servaddr) );
    if( bindOutput < 0 ) {
        printf("Cant bind\n");
        exit(0);
    }
    else {
        printf("Binded\n");
    }
    listen(sockFD , 5 );
    clientLen = sizeof(cliaddr);
    acceptOutput = accept(sockFD,(struct sockaddr *)&cliaddr , &clientLen);
    if(acceptOutput < 0) {
        printf("Cant accept\n");
        exit(0);
    }
    else {
        printf("Accepted\n");
    }
    char buffer[50] = "";
    while(1) {
        bzero(buffer, sizeof(buffer));
        read(acceptOutput , buffer , sizeof(buffer));
        printf("%d" , sizeof(buffer));
        //int len = sizeof(buffer);
        //substring(buffer , buffer , 0 , len);
        printf("\nMessage Received\t:\t%s\n" , buffer);
        if(strcmp(buffer ,"QUIT") == 0 ) {
            break;
        }
        write(acceptOutput , buffer , strlen(buffer));
        printf("\nMessage Sent\t:\t%s\n" , buffer);
    }
    close(sockFD);
    close(acceptOutput);
    return 0;
}
