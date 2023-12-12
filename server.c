#include "rdt.h"


int main(int argc, char *argv[]){

    struct addrinfo *res = initAddrInfo("8080");

    int sockfd = makeSockAndBindToPort(res);
    
    printMyIP(res);
    
    freeaddrinfo(res);

    char buffer[BUFFER_LENGTH];
    struct sockaddr theirAddr;
    socklen_t theirAddrLen = sizeof(theirAddr);

    receiveUDP(sockfd, &theirAddr, buffer);
    
    const char* message = "Message received.\n";
    sendUDP(
        sockfd,
        message,
        strlen(message),
        &theirAddr,
        theirAddrLen
    );

    close(sockfd);

    return 0;
}