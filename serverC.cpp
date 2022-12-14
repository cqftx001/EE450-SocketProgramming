//
//  serverC.cpp
//  csci455
//
//  Created by Qifan Chen on 11/28/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <strings.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <fstream>
#include <vector>
#include <sstream>

#define serverC_PORT 21624
#define maxLen 1024

using namespace std;
int main(int argc, char *argv[]){
    char empty[maxLen];
    char flagBack[maxLen];
    
    while(1){
        
        // server address and client address
        struct sockaddr_in serverC, serverM;
        
        char receivedMess[maxLen] = {};
        char Mess[maxLen] = {};
        socklen_t len;
        len = sizeof(serverM);
        
        //create socket & error check
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(sockfd<0){
            perror("Fail to create an UDP socket!\n");
            exit(1);
        }
        memset(&serverC, '\0', sizeof(serverC));
        serverC.sin_family = AF_INET;
        serverC.sin_port = htons(serverC_PORT);
        serverC.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        //bind the socket & error check
        int n = bind(sockfd, (struct sockaddr*)&serverC, sizeof(serverC));
        if(n < 0){
            perror("Fail to bind an UDP socket!\n");
            exit(1);
        }
        else{
            printf("The ServerC is up and running using UDP on port<%d>.\n", serverC_PORT);
        }
        
        //get the message & error check
        int s = recvfrom(sockfd, receivedMess, sizeof(receivedMess), 0, (struct sockaddr*)&serverM, &len);
        if(s < 0){
            printf("Fail to receive!\n");
            exit(1);
        }
        else{
            
            printf("The serverC received an anthentication request form the Main Server.\n");
            
            
            char *node;
            char *usernameAfterEncry;
            char *passwordAfterEncry;
            
            node = strtok(receivedMess, " ,");
            usernameAfterEncry = node;
            
            node = strtok(NULL, ",");
            passwordAfterEncry = node;
            receivedMess[s] = '\0';
            
            //input the file
            //选择比较cred里的文件
            
            
            char messageFromFile[maxLen];
            char *nodeInFile;
            char *usernameInFile;
            char *passwordInFile;
            
            string line;
            ifstream file("cred.txt");
            char sep = ',';
            
            if(file.is_open()){
                while(getline(file,line)){
                    
                    
                    strcpy(messageFromFile, line.c_str());
                    usernameInFile = strtok(messageFromFile, ",");
                    passwordInFile = strtok(NULL,",");
                    
                    if(strcmp(usernameAfterEncry, usernameInFile) == 0 && strcmp(passwordAfterEncry, passwordInFile) == 0){
                        strcpy(flagBack, "successful");
                        break;
                    }
                    else if(strcmp(usernameAfterEncry, usernameInFile) != 0){
                        strcpy(flagBack,"username");
                        continue;
                    }
                    else if(strcmp(usernameInFile, usernameAfterEncry) ==0 && strcmp(passwordAfterEncry, passwordInFile) != 0){
                        strcpy(flagBack, "password");
                        break;
                    }
                }
            }
            
                //send message to serverM
                int a = sendto(sockfd, flagBack, sizeof(flagBack), 0, (struct sockaddr*)&serverM, sizeof(serverM));
                if(a < 0){
                    perror("Fail to send an UDP socket!\n");
                    exit(0);
                }
                else{
                    printf("The ServerC finished sending the response to the Main Server.\n");
                }
            }
        close(sockfd);
        }
    return 0;
    }

                   
