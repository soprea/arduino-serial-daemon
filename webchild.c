#include "header.h"
#include "webchild.h"

#define MAXLINE                 3000
#define CONF_PATH               "."
#define LISTENQ 5

void web_child(int connfd){
    int n;
    char buffer[256];
    bzero(buffer,256);
    n = read(connfd,buffer,255);
    if (n < 0){
        perror("ERROR reading from socket");
        exit(1);
    }
    printf("Here is the message: %s\n",buffer);
    n = write(connfd,"I got your message",18);
    if (n < 0){
        perror("ERROR writing to socket");
        exit(1);
    }
    return 0;
}