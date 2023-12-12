#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>

#define BUFFER_LENGTH 500

void printMyIP(struct addrinfo* info){
    char addrStr[INET_ADDRSTRLEN];
    struct sockaddr_in* addr = (struct sockaddr_in*)info->ai_addr;
    inet_ntop(
        info->ai_family,
        &(addr)->sin_addr,
        addrStr,
        INET_ADDRSTRLEN
    );
    printf("My address: %s\n", addrStr);
}

void printIP(struct sockaddr_in* addr){
    char addrStr[INET_ADDRSTRLEN];
    inet_ntop(
        AF_INET,
        &(addr)->sin_addr,
        addrStr,
        INET_ADDRSTRLEN
    );
    printf("Address: %s\n", addrStr);
}

struct addrinfo* initAddrInfo(const char* port){
    //constructor for initializing 
    //struct addrinfo
    
    struct addrinfo hints;

    memset(&hints, 0, sizeof(hints));
    
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    
    struct addrinfo* res;
    if(getaddrinfo(NULL, port, &hints, &res) != 0){
        perror("Could not initialize addrinfo.");
        exit(1);
    }

    // printMyIP(res);
    return res;
}

int makeSockAndBindToPort(struct addrinfo* info){
    struct addrinfo* i;
    int sockfd;
    for(i = info; i != NULL; i = i->ai_next){
        int status;
        sockfd = socket(
            i->ai_family,
            i->ai_socktype,
            i->ai_protocol
        );
        if(sockfd == -1){continue;}
        
        int yes = 1;
        status = setsockopt(
            sockfd,
            SOL_SOCKET,
            SO_REUSEADDR,
            &yes,
            sizeof(int)
        );
        if(status == -1){continue;}

        status = bind(
            sockfd,
            i->ai_addr,
            i->ai_addrlen
        );
        if(status == -1){
            close(sockfd);
            continue;
        }

        break;
    }

    if(i == NULL){
        perror("Could not bind to port");
        exit(1);
    }

    // printMyIP(i);

    return sockfd;
}

ssize_t receiveUDP(int sockfd, struct sockaddr* theirAddr, char* buffer){
    socklen_t theirAddrLen = sizeof(theirAddr);
    ssize_t bytesRead = recvfrom(
        sockfd,
        buffer,
        BUFFER_LENGTH-1,
        0,
        theirAddr,
        &theirAddrLen
    );

    if(bytesRead == -1){
        perror("Could not receive data.");
    }

    printIP((struct sockaddr_in*)theirAddr);    

    buffer[bytesRead] = '\0';

    printf("%s\n", buffer);
    
    return bytesRead;
}

ssize_t sendUDP(int sockfd, const char* msg, int msgLen, 
            const struct sockaddr* toAddr, socklen_t toAddrLen){
    ssize_t bytesSent = sendto(
        sockfd,
        msg,
        strlen(msg),
        0,
        toAddr,
        toAddrLen
    );
    if(bytesSent == -1){
        perror("Could not send message");
    }
    return bytesSent;
}