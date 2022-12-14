//
//  serverEE.cpp
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
#define serverEE_PORT 23624
#define maxLen 1024

using namespace std;

int main(int argc, char *argv[]){
    
    char empty[maxLen];
    char information[maxLen];
    
    while(1){
        
        // server address and client address
        struct sockaddr_in serverEE, serverM;
        
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
        memset(&serverEE, '\0', sizeof(serverEE));
        serverEE.sin_family = AF_INET;
        serverEE.sin_port = htons(serverEE_PORT);
        serverEE.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        //bind the socket & error check
        
        if(bind(sockfd, (struct sockaddr*)& serverEE, sizeof(serverEE)) < 0){
            perror("Fail to bind an UDP socket!\n");
            exit(1);
        }
        else{
            printf("The Server<EE> is up and running using UDP on port<%d>.\n", serverEE_PORT);
        }
        
        //get the message & error check
        int s = recvfrom(sockfd, receivedMess, sizeof(receivedMess), 0, (struct sockaddr*)&serverM, &len);
        if(s < 0){
            printf("Fail to receive!\n");
            exit(1);
        }
        else{
            //divide the category and the course code
            //不太会
            char *node;
            char *courseCode;
            char *category;
            
            
            
            node = strtok(receivedMess, " ,");
            courseCode = node;
            
            node = strtok(NULL, " ,");
            category = node;
            
            printf("The server<EE> received a request from the Main Server about the <%s>of<%s>.\n", category, courseCode);
            receivedMess[s] = '\0';
            
            //input the file
            char messageFromFile[maxLen];
            char *nodeForFile;
            char *courseCodeForFile;
            char *creditForFile;
            char *professorForFile;
            char *daysForFile;
            char *coursenameForFile;
            
            char *categoryForFile;
            
            string line;
            ifstream file("ee.txt");
            char sep = ',';
            
            if(file.is_open()){
                while(getline(file,line)){
                    strcpy(messageFromFile, line.c_str());
                    courseCodeForFile = strtok(messageFromFile, ",");
        
                    if(strcmp(courseCode, courseCodeForFile) == 0){
                        
                        nodeForFile = strtok(NULL,",");
                        creditForFile = nodeForFile;
                        
                        nodeForFile = strtok(NULL,",");
                        professorForFile = nodeForFile;
                        
                        nodeForFile = strtok(NULL,",");
                        daysForFile = nodeForFile;
                        
                        nodeForFile = strtok(NULL,",");
                        coursenameForFile = nodeForFile;
                        
                        
                        if(strcmp(category,"Credit")){
                            strcpy(information,creditForFile);
                        }
                        else if (strcmp(category,"Professor")){
                            strcpy(information,professorForFile);
                        }
                        else if (strcmp(category,"Days")){
                            strcpy(information,daysForFile);
                        }
                        else if (strcmp(category,"CourseName")){
                            strcpy(information,coursenameForFile);
                        }
                        break;
                    }
                }
                if(strcmp(courseCode, courseCodeForFile) != 0){
                    printf("Didn't find the course:<%s>",courseCode);
                }
                
            }
            
            printf("The course information has been found: The<%s> of <%s> is <%s>.\n",category, courseCode, information);
            
            strcpy(Mess,"The <");
            strcat(Mess,category);
            strcat(Mess,"> of <");
            strcat(Mess,courseCode);
            strcat(Mess, "> is <");
            strcat(Mess,information);
            strcat(Mess,">.");
            
            //send message to serverM
            int a = sendto(sockfd, Mess, sizeof(Mess), 0, (struct sockaddr*)&serverM, sizeof(serverM));
            if(a < 0){
                perror("Fail to send an UDP socket!\n");
                exit(0);
            }
            else{
                printf("The Server<EE> finished sending the response to the Main Server.\n");
            }
        }
        close(sockfd);
    }
    return 0;
    }

