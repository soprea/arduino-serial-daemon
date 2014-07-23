#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include "arduino-serial-lib-daemon.h"
#include <mysql/mysql.h>

using namespace std;

#define DAEMON_NAME "arduino_server"
const int buf_max = 32;
char buf[buf_max];

void process(){
    FILE *fp;
    const int buf_string_max = 256;
    int fd = -1;
    char stat_db[buf_string_max], code_db[buf_string_max], db_buf[buf_string_max], buf_db[buf_string_max];
    const char delimiters[] = "C F#%";
    char *token1, *token2, *cp;
    sscanf(buf, "%s", buf_db);
    token1 = strtok (buf_db, delimiters);
    token2 = strtok (NULL, delimiters);

    MYSQL *conn;
    const char *server = "localhost";
    const char *user = "test";
    const char *password = "test";
    const char *database = "pihome";
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
      syslog(LOG_ERR, "%s", mysql_error(conn));}
   // exit(EXIT_FAILURE);
      sprintf(db_buf, "update pi_devices set status='%s' where code='%s';", token1, token2); // Puts entire query in a string
//    sprintf(db_buf, "update pi_devices set status='%s' where code='%s';", stat_db, code_db); // Puts entire query in a string
      syslog(LOG_INFO, "MySQL sent query: %s", db_buf);
    if (mysql_query(conn, db_buf)) { // send SQL query 
       syslog(LOG_ERR, "%s", mysql_error(conn));}
    // exit(EXIT_FAILURE);
       mysql_close(conn);
       fp = fopen("out.txt","a");
       fprintf(fp, "Read string: %s", buf); /*Write ouput in file*/
       fclose(fp);      
}   

int main(int argc, char *argv[]) {
    syslog(LOG_NOTICE, "Entering Daemon");
    syslog (LOG_INFO, "Program started by User %d", getuid ());
    pid_t pid, sid;
    pid = fork(); //Fork the Parent Process
    if (pid < 0) { syslog(LOG_ERR, "Can not create a new PID for our child process");}
    if (pid > 0) { exit(EXIT_SUCCESS); } //We got a good pid, Close the Parent Process
    umask(0); //Change File Mask
    sid = setsid(); //Create a new Signature Id for our child
    if (sid < 0) { syslog(LOG_ERR, "Can not create a new SID on child process");}
    if ((chdir("/")) < 0) { syslog(LOG_ERR, "Can not change directory on child process");}
    //Close Standard File Descriptors
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    //----------------
    //Main Process
    //----------------

while(true){
    FILE *fp;
    int fd = -1;
    int baudrate = 115200;  // default
    char eolchar = '\n'; // When arduino finishes sending message
    int timeout = 5000;
    char serialport[]="/dev/ttyACM0";
    fd = serialport_init(serialport, baudrate);
    if (fd == -1) {
      syslog(LOG_ERR, "Serial port not opened %s\n", serialport);}
   // exit(EXIT_FAILURE);
      syslog(LOG_INFO, "Serial port opened %s\n",serialport);
      memset(buf,0,buf_max);
      serialport_read_until(fd, buf, eolchar, buf_max, timeout);
      process();    //Run our Process
 //   sleep(1);    //Sleep for 60 seconds
	}
}
