#include "utils/header.h"
#include "mysql.h"
#define CONF_PATH "."

void mysql(char buf[32]) {
    const int buf_string_max = 256;
    char db_buf[buf_string_max], buf_db[buf_string_max];
    const char delimiters[] = "C F#%";
    char *token1, *token2;
    sscanf(buf, "%s", buf_db);
    token1 = strtok (buf_db, delimiters);
    token2 = strtok (NULL, delimiters);

    const char *server = "localhost";
    const char *user = "test";
    const char *password = "test";
    const char *database = "pihome";
    MYSQL *conn;

    conn = mysql_init(NULL);
      if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
        syslog(LOG_ERR, "%s", mysql_error(conn));
        exit(EXIT_FAILURE);}
      sprintf(db_buf, "update pi_devices set status='%s' where code='%s';", token1, token2); /* Puts entire query in a string */
      if (mysql_query(conn, db_buf)) { // send SQL query
        syslog(LOG_ERR, "%s", mysql_error(conn));
        exit(EXIT_FAILURE);} /* In case of MySQL sent query failure, close connection */
      syslog(LOG_INFO, "MySQL sent query: %s", db_buf);
      mysql_close(conn); /* Close MySQL Connection */
}
