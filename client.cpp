//
//  client.cpp
//  csci455
//
//  Created by Qifan Chen on 11/28/22.
//

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>

#define client_PORT 60000
#define maxLen 1024
using namespace std;
int main(int argc, char *argv[]){
    int count = 3;
    while(count != 0){
        char username[50];
        char password[50];
        
        int sockfd;
        struct sockaddr_in serverM;
        
        
        
        
        
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0){
            perror("Fail to create a a TCP socket!\n");
            exit(1);
        }
        bzero(&serverM, sizeof(serverM));
        
        serverM.sin_port = htons(client_PORT);
        serverM.sin_family = AF_INET;
        serverM.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        int n = connect(sockfd, (struct sockaddr*)&serverM, sizeof((serverM)));
        if(n < 0){
            perror("Fail to build connection!\n");
            close(sockfd);
            exit(1);
            
        }
        printf("The client is up and running.\n");
        
        int flag = 0;
        while(strlen(username)<5){
            if(flag == 1){
                printf("The length of the username should in [5,50]\n");
                strcpy(username,"");
            }
            printf("Please enter the username: <unencrypted_username>\n");
            std::cin >> username;
            flag = 1;
        }
        flag = 0;
        while(strlen(password)<5){
            if(flag == 1){
                printf("The length of the password should in [5,50]\n");
                strcpy(password,"");
            }
            printf("Please enter the password: <unencrypted_password>\n");
            std::cin >> password;
            flag = 1;
        }
        
        printf("<%s> sent an authentication request to the main server.\n", username);
        
        char empty[] = " ";
        char buff[maxLen];
        
        strcpy(buff,username);
        strcat(buff,",");
        strcat(buff,password);
        
        //send a message to serverM
        int sending = send(sockfd, buff, strlen(buff), 0);
        
        if(sending < 0){
            perror("Fail to send a TCP socket!\n");
            close(sockfd);
            exit(1);
        }
        
        char receiveMess[maxLen] = {};
        recv(sockfd, receiveMess, sizeof(receiveMess), 0);
        
        char *ptr;
        ptr = strtok(receiveMess, " ,");
        
        //successfully match
        if(strcmp(ptr,"successful") == 0){
            printf("<%s> received the result of authentication using TCP over port<%d>. Authentication is successful!\n", username, client_PORT);
            break;
        }
        else if (strcmp(ptr,"password") == 0 && count != 0){
            printf("<%s> received the result of authentication using TCP over port<%d>. Authentication failed: Password does not match!\n", password, client_PORT);
            count--;
            printf("Attempts remaining:<%d>",count);
            if(count==0){
                printf("Authentication Failed for 3 attempts. Client will shut down.");
                exit(0);
            }
        }
        else if(strcmp(ptr,"username") == 0 && count != 0){
            printf("<%s> received the result of authentication using TCP over port<%d>. Authentication failed: Username does not match!\n", username, client_PORT);
            count--;
            printf("Attempts remaining:<%d>",count);
            if(count==0){
                printf("Authentication Failed for 3 attempts. Client will shut down.");
                exit(0);
            }
        }
        
        //request the input course
        char courseCode[maxLen];
        char category[maxLen];
        char twoMessage[maxLen];
        printf("Please enter the course code to query");
        cin>>courseCode;
        
        printf("Please enter the category (Credit / Professor / Days / CourseName):");
        cin>> category;
        
        printf("<%s> sent a request to the main server.",username);
        
        strcpy(twoMessage,courseCode);
        strcat(twoMessage,",");
        strcat(twoMessage,category);
        
        //send a message to serverM
        int send2 = send(sockfd, twoMessage, strlen(buff), 0);
        
        if(send2 < 0){
            perror("Fail to send a TCP socket!\n");
            close(sockfd);
            exit(1);
        }
        
        char twoMessageRece[maxLen] = {};
        recv(sockfd, twoMessageRece, sizeof(twoMessageRece), 0);
        printf("The client received the response from the Main server using TCP over port <%d>",client_PORT);
        char *q;
        
        
        
        
    }
 
    
    
}
