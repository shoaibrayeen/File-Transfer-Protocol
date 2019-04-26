#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>    
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#define PORT 12345




void listFunction(char buffer[]) {
    DIR *obj;
    struct dirent *dir;
    obj = opendir(".");
    strcat(buffer , "\nFiles or Directories in Current Directory\n");
    if(obj) {
        while( (dir = readdir(obj) ) != NULL ) {
            strcat(buffer , dir->d_name );
            strcat(buffer , "\n");
        }
    }
}

void pwdFunction(char buffer[]) {
    char pwd[PATH_MAX];
    if (getcwd(pwd, sizeof(pwd)) != NULL) {
        strcat(buffer , "\nCurrent Working Directory\t:\t");
        strcat(buffer , pwd);
        strcat(buffer , "\n");
    } 
    else {
        strcpy(buffer , "\nCan't fetch current directory right now\n");
        return;
    }
}
void cwdFunction(char buffer[] , char temp[]) {
    if( temp[3] != ' ') {
        strcat(buffer , "\n504 	Command not implemented for that parameter.\n");
        return;
    }
    int len = strlen(temp);
    len -= 4;
    char path[len];
    strncpy(path , temp + 4 , len);
    path[len] = '\0';
    if (chdir(path) != 0)  {
        strcat(buffer , "\nNo Such File or Directory\n");
        return;
    }  
}

void mkdFunction(char buffer[] , char temp[]) {
    if( temp[3] != ' ') {
        strcat(buffer , "\n504 	Command not implemented for that parameter.\n");
        return;
    }
    int len = strlen(temp);
    len -= 4;
    char path[len];
    int i , j;
    for(i = 0; i < len; i++ ) {
        if(temp[ i + 4 ] == ' ') {
            strcat(buffer ,"\nInvalid Path\n");
            return;
        }
        if(temp[i+4] == '/' && i != len){
            j = i;
        }
        path[i] = temp[i+4];
    }
    path[j] = '\0';
    char dirName[len - j];
    strncpy(dirName , temp + 5 + j  , len - j);
    if (chdir(path) != 0)  {
        strcat(buffer , "\nNo Such File or Directory\n");
        return;
    }
    pwdFunction(buffer);
    if (mkdir(dirName , 0777) == -1)  {
        strcat(buffer , "\nError in Directory Creation\n"); 
        return;
    }
    else {
        strcat(buffer , "\nDirectory created\n"); 
        pwdFunction(buffer);
    }
}

void removeDirectory(const char path[] , char buffer[]) {
    size_t path_len;
    char *full_path;
    DIR *dir;
    struct stat stat_path, stat_entry;
    struct dirent *entry;
    stat(path, &stat_path);
    if (S_ISDIR(stat_path.st_mode) == 0) {
        strcat(buffer , "\nNo Such File or Directory\n");
        return;
    }
    if ((dir = opendir(path)) == NULL) {
        strcat(buffer , "\nCan't Open Given Directory\n");
        return;
    }
    path_len = strlen(path);
    // for each entry in Directory
    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, "..")) {
            continue;
        }
        full_path = calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
        strcpy(full_path, path);
        strcat(full_path, "/");
        strcat(full_path, entry->d_name);
        stat(full_path, &stat_entry);
        if (S_ISDIR(stat_entry.st_mode) != 0) {
            removeDirectory(full_path , buffer);
            continue;
        }
        if (unlink(full_path) == 0) {
            strcat(buffer , "\nFile Removed\t\t:\t");
            strcat(buffer , full_path);
        }
        else {
            strcat(buffer , "\ncan't remove File\t:\t");
            strcat(buffer , full_path);
            //return;
        }
    }
    if (rmdir(path) == 0) {
        strcat(buffer , "\nDirectory Removed\t:\t");
        strcat(buffer , path);
    }
    else {
        strcat(buffer , "\nCan't remove Directory\t:\t");
        strcat(buffer , full_path);
        //return;
    }
    closedir(dir);
}
void rmdFunction(char buffer[] , char temp[]) {
    if( temp[3] != ' ') {
        strcat(buffer , "\n504 	Command not implemented for that parameter.\n");
        return;
    }
    int len = strlen(temp);
    len -= 4;
    char path[len];
    strncpy(path , temp + 4 , len);
    path[len] = '\0';
    
    removeDirectory(path , buffer);
    
}
char fileName[50];
void storFunction(char buffer[] , char temp[]) {
    int len = strlen(temp) - 5;
    
    strncpy(fileName , temp + 5 , len);
    fileName[len] = '\0';
    //Creating File
    FILE *fPtr;
    fPtr = fopen(fileName , "w");
    if (fPtr == NULL) {
        strcat(buffer , fileName);
        strcat(buffer , "\t:\t Does Not Exist\n");
        return;
    }
    strcpy(buffer , "\n225 	Data connection open; no transfer in progress.\n");
    fclose(fPtr);
}

void retrFunction(char buffer[] , char temp[] ) {
    int len = strlen(temp) - 5;
    strncpy(fileName , temp + 5 , len);
    fileName[len] = '\0';
    FILE *fPtr;
    fPtr = fopen(fileName , "r");
    if (fPtr == NULL) {
        strcat(buffer , fileName);
        strcat(buffer , "\t:\t Does Not Exist\n");
        return;
    }
    strcpy(buffer , "\n226 	Data connection open; no transfer in progress.\n");
    fclose(fPtr);
}

int find(char* str1) {
    if(str1[0] == 'U') {
        if(str1[1] == 'S' && str1[2] == 'E' && str1[3] == 'R') {
            return 1;
        }
        return 0;
    }
    else if(str1[0] == 'P') {
        if(str1[1] == 'A' && str1[2] == 'S' && str1[3] == 'S') {
            return 2;
        }
        else if(str1[1] == 'W' && str1[2] == 'D') {
            return 6;
        }
        else {
            return 0;
        }
    }
    else if(str1[0] == 'M') {
        if(str1[1] == 'K' && str1[2] == 'D') {
            return 3;
        }
        return 0;
    }
    else if(str1[0] == 'C') {
        if(str1[1] == 'W' && str1[2] == 'D') {
            return 4;
        }
        return 0;
    }
    else if(str1[0] == 'R') {
        if(str1[1] == 'E' && str1[2] == 'T' && str1[3] == 'R') {
            return 7;
        }
        else if(str1[1] == 'M' && str1[2] == 'D') {
            return 5;
        }
        else {
            return 0;
        }
    }
    else if(str1[0] == 'S') {
        if(str1[1] == 'T' && str1[2] == 'O' && str1[3] == 'R') {
            return 8;
        }
        return 0;
    }
    else if(str1[0] == 'L') {
        if(str1[1] == 'I' && str1[2] == 'S' && str1[3] == 'T') {
            return 9;
        }
        return 0;
    }
    else if(str1[0] == 'A') {
        if(str1[1] == 'B' && str1[2] == 'O' && str1[3] == 'R') {
            return 10;
        }
        return 0;
    }
    else if(str1[0] == 'Q') {
        if(str1[1] == 'U' && str1[2] == 'I' && str1[3] == 'T') {
            return 11; 
        }
        return 0;
    }
    return 0;
}

int flag = 0;
int indexDB = 0;
void callFunction(int returnValue, char buffer[]) {
    char temp[500] = "";
    strcpy(temp , buffer);
    strcpy(buffer , "\n200	Command okay\n");
    if(returnValue == 1) {
        if( flag == 1 || flag == 2) {
            strcpy(buffer , "\n503 	Bad sequence of commands.\n");
            return;
        }
        if( temp[4] != ' ') {
            strcpy(buffer , "\n503 	Bad sequence of commands.\n");
            return;
        }
        char userID[10];
        strncpy(userID , temp + 5 , 9);
        static const char filename[] = "idPass.txt";
        FILE *file = fopen ( filename, "r" );
        char line [ 100 ]; /* or other suitable maximum line size */
        char id[10];
        while ( fgets ( line, sizeof line, file ) != NULL ) {
            strncpy(id , line , 9);
            id[9] = '\0';
            indexDB++;
            if ( strcmp( id , userID ) == 0 ) {
                flag = 1;
                break;
            }
        }
        fclose ( file );
        if ( flag == 1) {
            strcat(buffer , "\n331 	User name okay, need password.\n");
        }
        else {
            strcpy(buffer , "\nUserID not Found In DATABASE\n");
            indexDB = 0;
            return;
        }
        /*for(int i = 5; i < 11; i++ ) {
            if( (temp[i] - '0') != i - 4 ) {
                strcpy(buffer , "\nIncorrect UserId\n");
                return;
            }
        */
        /*
        strcat(buffer , "\n331 	User name okay, need password.\n");
        flag = 1;
        */
    }
    else if ( returnValue == 2) {
        if( flag == 2 || flag == 0) {
            strcpy(buffer , "\n503 	Bad sequence of commands.\n");
            return;
        }
        if( temp[4] != ' ') {
            strcpy(buffer , "\n503 	Bad sequence of commands.\n");
            return;
        }
        /*
        for(int i = 5; i < 11; i++ ) {
            if( (temp[i] - '0') != i - 4 ) {
                strcat(buffer , "\nIncorrect Password\n");
                return;
            }
        }
        strcat(buffer , "\nSuccessfully Logged In.\n");
        flag = 2;
        */
        char userPass[9];
        strncpy(userPass , temp + 5 , 8);
        static const char filename[] = "idPass.txt";
        FILE *file = fopen ( filename, "r" );
        char line [ 100 ]; /* or other suitable maximum line size */
        char pass[9];
        int count = 1;
        while ( fgets ( line, sizeof line, file ) != NULL ) {
            if( count == indexDB ) {
                strncpy(pass , line + 10 , 8);
                pass[8] = '\0';
                if ( strcmp( pass , userPass ) == 0 ) {
                    flag = 2;
                }
                break;
            }
            count += 1;
        }
        fclose ( file );
        if ( flag == 2) {
            strcat(buffer , "\nSuccessfully Logged In.\n");
            return;
        }
        else {
            strcpy(buffer , "\nIncorrect Password\n");
            return;
        }
    }
    else if ( returnValue == 3) {
        if(flag == 0) {
            strcat(buffer , "\n530 	Not logged in.\n");
            return;
        }
        if(flag == 1) {
            strcat(buffer , "\n331 	User name okay, need password.\n");
            return;
        }
        mkdFunction(buffer , temp);
    }
    else if ( returnValue == 4) {
        if(flag == 0) {
            strcat(buffer , "\n530 	Not logged in.\n");
            return;
        }
         if(flag == 1) {
            strcat(buffer , "\n331 	User name okay, need password.\n");
            return;
        }
        pwdFunction(buffer);
        cwdFunction(buffer, temp);
        pwdFunction(buffer);
    }
    else if ( returnValue == 5) {
        if(flag == 0) {
            strcat(buffer , "\n530 	Not logged in.\n");
            return;
        }
         if(flag == 1) {
            strcat(buffer , "\n331 	User name okay, need password.\n");
            return;
        }
        rmdFunction(buffer , temp);
    }
    else if ( returnValue == 6) {
        if(flag == 0) {
            strcat(buffer , "\n530 	Not logged in.\n");
            return;
        }
         if(flag == 1) {
            strcat(buffer , "\n331 	User name okay, need password.\n");
            return;
        }
        if( temp[3] == ' ') {
            strcat(buffer , "\n504 	Command not implemented for that parameter.\n");
            return;
        }
        if( strlen(temp) > 3) {
            strcpy(buffer , "\n503 	Bad sequence of commands.\n");
            return;
        }
        pwdFunction(buffer);
    }
    else if ( returnValue == 7) {
        if(flag == 0) {
            strcat(buffer , "\n530 	Not logged in.\n");
            return;
        }
         if(flag == 1) {
            strcat(buffer , "\n331 	User name okay, need password.\n");
            return;
        }
         if( temp[4] != ' ') {
            strcpy(buffer , "\n503 	Bad sequence of commands.\n");
            return;
        }
        retrFunction(buffer , temp);
    }
    else if ( returnValue == 8) {
        if(flag == 0) {
            strcat(buffer , "\n530 	Not logged in.\n");
            return;
        }
         if(flag == 1) {
            strcat(buffer , "\n331 	User name okay, need password.\n");
            return;
        }
        if( temp[4] != ' ') {
            strcpy(buffer , "\n503 	Bad sequence of commands.\n");
            return;
        }
        storFunction(buffer , temp);
    }
    else if ( returnValue == 9) {
        if(flag == 0) {
            strcat(buffer , "\n530 	Not logged in.\n");
            return;
        }
         if(flag == 1) {
            strcat(buffer , "\n331 	User name okay, need password.\n");
            return;
        }
        if( temp[4] == ' ') {
            strcat(buffer , "\n504 	Command not implemented for that parameter.\n");
            return;
        }
        if( strlen(temp) > 4) {
            strcpy(buffer , "\n503 	Bad sequence of commands.\n");
            return;
        }
        listFunction(buffer);
    }
    else if ( returnValue == 10) {
        if(flag == 0) {
            strcat(buffer , "\n530 	Not logged in.\n");
            return;
        }
         if(flag == 1) {
            strcat(buffer , "\n331 	User name okay, need password.\n");
            return;
        }
        if( temp[4] == ' ') {
            strcat(buffer , "\n504 	Command not implemented for that parameter.\n");
            return;
        }
        if( strlen(temp) > 4) {
            strcat(buffer , "\n503 	Bad sequence of commands.\n");
            return;
        }
        flag = 0;
        strcat(buffer , "\n551 	Requested action aborted: page type unknown. - User Logged Out\n");
        indexDB = 0;
    }
    else if ( returnValue == 11) {
        strcpy(buffer ,  "\nConnection Termination\n");
        return;
    }

}

void mainloop(int acceptOutput) {
    char buffer[10000] = "";
    char temp[10000] = " ";
    while(1) {
        if( strcmp(temp , "\n225 	Data connection open; no transfer in progress.\n") == 0) {
            FILE *fPtr;
            fPtr = fopen(fileName, "w"); 
            if (fPtr == NULL) { 
                printf("Cannot open file %s \n", fileName);  
            }
            else {
                bzero(buffer, sizeof(buffer));
                while (1) {
                    read(acceptOutput , buffer , sizeof(buffer));
                    fputs(buffer , fPtr);
                    if ( strlen(buffer) - 1 != sizeof(buffer) ) {
                        break;
                    }
                    bzero(buffer, sizeof(buffer));
                }
                printf("\nFile Received Successfully\n");
                fclose(fPtr);
                bzero(temp , sizeof(temp));
            }
        }
        bzero(buffer, sizeof(buffer));
        read(acceptOutput , buffer , sizeof(buffer));
        printf("\nMessage Received\t:\t%s\n" , buffer);
        
        int returnValue = find( buffer );
        if( returnValue == 0 ) {
            strcpy(buffer , "502 	Command not implemented\n");
        }
        else {
            callFunction(returnValue , buffer);
        }
        write(acceptOutput , buffer , strlen(buffer));
        if( strcmp( "\nConnection Termination\n" , buffer) == 0) {
            //sleep(3);
            printf("%s" , buffer);
            exit(0);
        }
        printf("%s" , buffer);
        strcpy(temp , buffer);


        if ( strcmp(temp , "\n226 	Data connection open; no transfer in progress.\n") == 0 ) {
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
                        write(acceptOutput , buffer, strlen(buffer));
                        break;
                    }
                    if ( strlen(buffer) == sizeof(buffer) ) {
                        write(acceptOutput , buffer, strlen(buffer));
                        bzero(buffer, sizeof(buffer));
                        i = 0;
                    }
                }
                printf("\nFile Sent Successfully\n");
                fclose(fPtr);
            }
        }
    }
}

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
    while(1) {
        clientLen = sizeof(cliaddr);
        acceptOutput = accept(sockFD,(struct sockaddr *)&cliaddr , &clientLen);
        if(acceptOutput < 0) {
            printf("Cant accept\n");
            exit(0);
        }
        else {
            printf("Accepted\n");
        }
        pid_t pid;
        if( (pid = fork()) == 0) {
            close(sockFD);
            mainloop(acceptOutput);
            close(acceptOutput);
            exit(0);
        }
        close(acceptOutput);
    }
    return 0;
}
