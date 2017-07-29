#include "utils/header.h"
#include "webchild.h"

#define MAXLINE                 3000
#define CONF_PATH               "."
#define LISTENQ 5

void web_child(int connfd, int fd){
    int n;
    char buffer[256];
    bzero(buffer,256);
    n = read(connfd,buffer,255);
    if (n < 0){
        perror("Webchild: ERROR reading from socket");
        exit(1);
    }
    //printf("Here is the message: %s\n",buffer);
    //strncat(buffer,"\n", sizeof buffer);
    //n = write(connfd,"I got your message",18);
        serialport_write(fd, buffer); //I have to error check this
        FILE *fp = fopen("out1.txt","a"); /* Open file in append monde */
        fprintf(fp, "Read string: %s", buffer); /* Write output in file */
        fclose(fp);/* Close file */
    if (n < 0){
        perror("Webchild: ERROR writing to socket");
        exit(1);
    }
    return;
}
