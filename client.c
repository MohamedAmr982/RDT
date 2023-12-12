#include "rdt.h"

int main(int argc, char* argv[]){


    struct addrinfo *res = initAddrInfo("8081");

    int sockfd = makeSockAndBindToPort(res);

    printMyIP(res);

    freeaddrinfo(res);

    char buffer[BUFFER_LENGTH];
    struct addrinfo* theirInfo = initAddrInfo("8080");
    struct sockaddr theirAddr = *theirInfo->ai_addr;
    socklen_t theirAddrLen = sizeof(theirAddr);

    const char* message = "connect\n";
    
    sendUDP(
        sockfd,
        message,
        strlen(message),
        &theirAddr,
        theirAddrLen
    );

    receiveUDP(sockfd, &theirAddr, buffer);

    close(sockfd);

    return 0;
}