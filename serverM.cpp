//
//  serverM.cpp
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

#define serverM_UDP_PORT 24624
#define serverM_TCP_PORT 25624
#define serverEE_PORT 23624
#define serverCS_PORT 22624
#define client_PORT 60000
#define serverC_PORT 21624

#define maxLen 1024

using namespace std;


//UDP
int serverEE_sock;
int serverCS_sock;
int serverC_sock;
//TCP
int sock;

struct sockaddr_in serverM_addr, serverEE_addr, serverCS_addr, client_addr, serverC_addr;
socklen_t Clientaddr;


char messageSent_serverC[maxLen];
char messageRece_serverC[maxLen];
socklen_t Caddr;

//communicaton with client
int child_sock;
int rval;
char buff[maxLen];
char receiveMess[maxLen];
char sendBackMess[maxLen];

//communication with serverEE
char buff1[maxLen];
int length;
char messageSent_serverEE[maxLen];
char messageRece_serverEE[maxLen];
socklen_t EEaddr;

//communication with serverCS
char buff2[maxLen];
char messageSent_serverCS[maxLen];
char messageRece_serverCS[maxLen];
socklen_t CSaddr;

//TCP socket method blocks(ini/bind/listen..)
void initTCPsock(){
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        printf("Fail to create a TCP socket!\n");
        exit(1);
    }
}

//bind
void bindTCPsock(){
    serverM_addr.sin_family = AF_INET;
    serverM_addr.sin_port = htons(client_PORT);
    serverM_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if(bind(sock, (struct sockaddr*)&serverM_addr, sizeof(serverM_addr)) < 0){
        perror("Fail to bind a TCP socket!\n");
        exit(1);
    }
}

//listen
void listenTCPsock(){
    //for now TCP socket is done
    listen(sock, 6);
    printf("The main server is up and running.\n");
}

//UDP socket method blocks
void initserverEEsock(){
    serverEE_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(serverEE_sock < 0){
        perror("Fail to create serverEE socket!\n");
        exit(1);
    }
}

char* Encry(char *strl, int numB, int model){
    
    if(model == 1){
        for(int i=0; i<strlen(strl);i++){
            if(strl[i] >= 'A' && strl[i] <= 'Z'){
                strl[i] = ((strl[i]-'A')+numB)%26 +'A';
            }
            else if(strl[i] >= 'a' && strl[i] <= 'z'){
                strl[i] = ((strl[i]-'a')+numB)%26 +'a';
            }
        }
        
    }
    return strl;
}

void acceptTCPsock(){
    Clientaddr = sizeof(client_addr);
    while(1){
        child_sock = accept(sock, (struct sockaddr*)&client_addr, &Clientaddr);
        
        if(child_sock < 0){
            perror("Fail to accept the child socket!\n");
        }
        recv(child_sock, receiveMess, sizeof(receiveMess), 0);
        
        char *username;
        char *password;
        char unencry[maxLen];
        char encryedTotal[maxLen];
        
        char encriptedUsername[maxLen];
        char encriptedPassword[maxLen];
        
        strcpy(unencry, receiveMess);
        username = strtok(unencry,",");
        password = strtok(NULL, ",");
        strcpy(encriptedUsername,Encry(username, 4, 1));
        strcpy(encriptedPassword,Encry(password, 4, 1));
        
        //combine and send
        strcpy(encryedTotal, encriptedUsername);
        strcat(encryedTotal,",");
        strcat(encryedTotal,encriptedPassword);
        
        printf("The main server received the result of the authentication for<%s> using TCP over port<21624>",username);
        
        
        
    }
}


//send message
void sendserverEEsock(){
    memset(&serverEE_addr, 0, sizeof(serverEE_addr));
    serverEE_addr.sin_family = AF_INET;
    serverEE_addr.sin_port = htons(serverEE_PORT);
    serverEE_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int send = sendto(serverEE_sock, messageSent_serverEE, sizeof(messageSent_serverEE), 0, (struct sockaddr*)&serverEE_addr, sizeof(serverEE_addr));
    if(send < 0){
        perror("Fail to send an UDP socket!\n");
        exit(1);
    }
    printf("The main server sent a request to server<EE>");
}

//receive message
void receserverEEsock(){
    int n = recvfrom(serverEE_sock, messageRece_serverEE, sizeof(messageRece_serverEE), 0, (struct sockaddr*)&serverEE_addr, &EEaddr);
    if(n<0){
        printf("Fail to receive an UDP socket!\n");
        exit(0);
    }
    else{
        messageRece_serverEE[n] = '\0';
    }
    char mess[maxLen];
    char *node;
    
    strcpy(mess,messageRece_serverEE);
    printf("The main server received the response from server<EE> using UDP over port<23624>");

}







void initserverCSsock(){
     if((serverCS_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
         perror("UDP SOCKETCS CREATE FAILED");
         exit(1);
     }
}

void sendserverCSsock(){
    memset(&serverCS_addr, 0 , sizeof(serverCS_addr));
     serverCS_addr.sin_family = AF_INET;
     serverCS_addr.sin_port = htons(serverCS_PORT);
     serverCS_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

     int sent_succesful = sendto(serverCS_sock, messageSent_serverCS, sizeof(messageSent_serverCS), 0, (struct sockaddr *)& serverCS_addr, sizeof(serverCS_addr));
     if(sent_succesful < 0){
         perror("UDPCS SENT FALIED");
     }
    printf("The main server sent a request to server<CS>");
}

void receiveserverCSsock(){
    int n = recvfrom(serverCS_sock, messageRece_serverCS, sizeof(messageRece_serverCS), 0, (struct sockaddr* )&serverCS_addr, &CSaddr);
     if(n < 0){
        printf("UDPCS Receive failed\n");
        exit(0);
    }
    else{
        messageSent_serverCS[n] = '\0';
    }
    
    printf("The main server received the response from server<CS> using UDP over port<22624>");
    char message[maxLen];
    strcpy(message, messageSent_serverCS);

}

void initserverCsock(){
     if((serverC_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0){
         perror("UDP SOCKETC CREATE FAILED");
         exit(1);
     }
}

void sendserverCsock(){
    memset(&serverC_addr, 0 , sizeof(serverC_addr));
     serverC_addr.sin_family = AF_INET;
     serverC_addr.sin_port = htons(serverC_PORT);
     serverC_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

     int sent_succesful = sendto(serverC_sock, messageSent_serverC, sizeof(messageSent_serverC), 0, (struct sockaddr *)& serverC_addr, sizeof(serverC_addr));
     if(sent_succesful < 0){
         perror("UDPC SENT FALIED");
     }
    printf("The main server sent a request to server<CS>");
}

void receiveserverCsock(){
    int n = recvfrom(serverC_sock, messageRece_serverC, sizeof(messageRece_serverC), 0, (struct sockaddr* )&serverC_addr, &Caddr);
     if(n < 0){
        printf("UDPC Receive failed\n");
        exit(0);
    }
    else{
        messageSent_serverC[n] = '\0';
    }
    
    printf("The main server received the response from serverC using UDP over port<21624>");
    char message[maxLen];
    strcpy(message, messageSent_serverC);

}
int main(){
    
    initTCPsock();
    bindTCPsock();
    listenTCPsock();
    acceptTCPsock();
    
    return 0;
}
