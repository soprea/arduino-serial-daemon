#include "header.h"
#include "config.h"
#include "webchild.h"

#define DAEMON_NAME "arduino-serial-daemon"
#define LISTENQ 5
#define MAXLINE 3000


const int buf_max = 32;
char buf[32];

void sig_chld(int signo) {
    pid_t pid;
    int stat;
    char line[MAXLINE];

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0) {
        sprintf(line, "child %d terminated", pid);
        syslog(LOG_INFO, line);
    }
    return;
}
void process(){
    FILE *fp;
    struct tm *sTm;
    char buff_time[50];
    time_t now = time (0);
    sTm = localtime (&now);
    strftime (buff_time, sizeof(buff_time), "%z %Y-%m-%d %A %H:%M:%S", sTm);
    fp = fopen("out.txt","a"); /* Open file in append monde */
    fprintf(fp, "%s Read string: %s", buff_time, buf); /* Write output in file */
    fclose(fp);/* Close file */
}
void read_serial(){
        /* Serial */
    int fd = -1, baudrate = 115200, timeout = 5000;
    char serialport[]="/dev/ttyACM0", eolchar = '\n';
    fd = serialport_init(serialport, baudrate);
    if (fd == -1) { syslog(LOG_ERR, "Serial port not opened %s\n", serialport); exit(EXIT_FAILURE);}
    syslog(LOG_INFO, "Serial port opened %s\n",serialport);
    while (1){
        memset(buf,0,buf_max);
        serialport_read_until(fd, buf, eolchar, buf_max, timeout);
        process();
    }
}
void daemonize(void){
    syslog(LOG_NOTICE, "Entering Daemon");
    syslog (LOG_INFO, "Program started by User %d", getuid ());
    pid_t pid, sid;
    if (getppid() == 1) return; /* Already a daemon */
    pid = fork(); //Fork the Parent Process
    if (pid < 0) { syslog(LOG_ERR, "Can not create a new PID for our child process");}
    if (pid > 0) { exit(EXIT_SUCCESS); } /* We got a good pid, Close the Parent Process */
    umask(0); /* Change File Mask */
    sid = setsid(); /* Create a new Signature Id for our child */
    if (sid < 0) { syslog(LOG_ERR, "Can not create a new SID on child process");}
    if ((chdir("/")) < 0) { syslog(LOG_ERR, "Can not change directory on child process");}
    /* Close Standard File Descriptors: */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}
int main(int argc, char *argv[]) {
    int listenfd;
    int connfd;
    socklen_t clilen;
    socklen_t addrlen;
    struct sockaddr_in servaddr;
    struct sockaddr cliaddr;
    pid_t childpid;
    struct conf parms;

    /* configuration file */
    init_parameters(&parms);
    parse_config(&parms);

    /* read the authorised user file */

    daemonize();
    /* socket */

    //creation of the socket
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    //preparation of the socket address
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_addr.s_addr = inet_addr(parms.ListenIP);
    servaddr.sin_port = htons(atoi(parms.ListenPort));

    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof (servaddr)) < 0) {
        exit(EXIT_FAILURE);
    }
    listen(listenfd, LISTENQ);
    syslog(LOG_INFO, "Server running...waiting for connections.");
    signal(SIGCHLD, sig_chld);
    /* accept , cycle , then fork exec */
while(1){
      if((childpid = fork()) == 0) { /* child process */
         read_serial();
       }
        // continue;}
        clilen = addrlen = sizeof (struct sockaddr_in);
        if ((connfd = accept(listenfd, &cliaddr, &clilen)) < 0) {
            syslog(LOG_INFO, "accept <0");
            if (errno == EINTR)
                continue; /* back to for() */
            else {
                syslog(LOG_INFO, "accept error");
                continue;
            }
        }
        if ((childpid = fork()) == 0) { /* child process */
            close(listenfd); /* close listening socket */
            web_child(connfd); /* process request */
            //    close(connfd);
            exit(0);
        }
        close(connfd); /* parent closes connected socket */
    }
    return 0;
}