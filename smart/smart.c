#include "../webserver/webserver.h"
#include "../config.h"
#include "../header.h"

struct conf parms;
/* configuration file */
//static char keypressed[10];

void smart() {
  //Doing the smart moves;
  //Send them with sendKey(keypressed);
}

void do_row(){
  // move
}

void do_column(){
  //
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

int main(void) {
  init_parameters(&parms);
  parse_config(&parms);
  daemonize();
  smart();
  return 0;
}
