#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
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
    char buffer[10000] = "";
    char fileName[30];
    while(1) {
        bzero(buffer, sizeof(buffer));
        printf("\n$\t:\t");
        scanf("%[^\n]%*c" , buffer);
        write(sockFD , buffer, strlen(buffer));
        printf("\nMessage Sent\t:\t%s\n" , buffer);
        if ( strlen(buffer) > 7) {
            int len = strlen(buffer) - 5;
            strncpy(fileName , buffer + 5 , len);
            fileName[len] = '\0';
        }
        read(sockFD , buffer , sizeof(buffer));
        if(strcmp(buffer ,"\nConnection Termination\n") == 0 ) {
            printf("\nConnection Termination\n");
            break;
        }
        if( strcmp(buffer , "\n225 	Data connection open; no transfer in progress.\n") == 0) {
            printf("%s" , buffer);

            printf("\nFile Sending In Process\n");
            sleep(2);
            FILE *fPtr;
            fPtr = fopen(fileName, "r"); 
            if (fPtr == NULL) { 
                printf("Cannot open file\t:\t%s \n", fileName); 
            }
            else {
                char ch;
                bzero(buffer, sizeof(buffer));
                int i = 0;
                while(1) {
                    while ( ( (ch = fgetc(fPtr)) != EOF ) && ( strlen(buffer) - 1 != sizeof(buffer)) ) {
                        buffer[i] = ch;
                        i++;
                    }
                    buffer[i] = '\0';
                    if ( ch == EOF) {
                        write(sockFD , buffer, strlen(buffer));
                        break;
                    }
                    if ( strlen(buffer) == sizeof(buffer) ) {
                        write(sockFD , buffer, strlen(buffer));
                        bzero(buffer, sizeof(buffer));
                        i = 0;
                    }
                }
                printf("\nFile Sent Successfully\n");
                fclose(fPtr);
            }
        }
        else if ( strcmp(buffer , "\n226 	Data connection open; no transfer in progress.\n") == 0) {
            printf("%s" , buffer);
            FILE *fPtr;
            fPtr = fopen(fileName, "w"); 
            if (fPtr == NULL) { 
                printf("Cannot open file\t:\t%s \n", fileName);  
            }
            else {
                bzero(buffer, sizeof(buffer));
                while (1) {
                    read(sockFD , buffer , sizeof(buffer));
                    fputs(buffer , fPtr);
                    if ( strlen(buffer) != sizeof(buffer) ) {
                        break;
                    }
                    bzero(buffer, sizeof(buffer));
                }
                printf("\nFile Received Successfully\n");
                fclose(fPtr);
            }
        }
        else {
            printf("%s" , buffer);
        }
    }
    close(sockFD);

    return 0;
}
