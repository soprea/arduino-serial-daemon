#include "header.h"
#include "config.h"

#define CONFIG_FILE             "/etc/ardubot.conf" /* ardubot configuration file*/
#define CONF_STRING_LEN         100
#define DEF_SERVER_IP           "127.0.0.1" /* ardubot listening ip for webserver to send the keys */
#define DEF_LISTEN_IP           "127.0.0.1" /* ardubot listening ip */
#define DEF_SERVER_PORT         "4000" /* ardubot listening port for webserver to send the keys */
#define DEF_LISTEN_PORT         "4000"  /* ardubot listening port */
#define DEF_SERIAL_PORT         "/dev/ttyACM0" /* arduino serial port */
#define MAXLINE                 3000
#define DEF_WEB_ROOT            "/root/ardubot/web_root/" /* webserver root directory */
#define DEF_WEB_PORT            "6080" /* port for webserver */
#define DEF_SER_BAUD_RATE       "115200"
#define DEF_SMART_ROWS_D        "5" /* distance between rows */
#define DEF_SMART_COLUMNS_D     "5" /* distance between columns */
#define DEF_SMART_ROWS_NUM      "5" /* how many rows */
#define DEF_SMART_COLUMNS_NUM   "20" /* how many columns */


/* initialise data to default values */
void init_parameters (struct conf * parms) {
  strncpy (parms->ListenIP, DEF_LISTEN_IP, CONF_STRING_LEN);
  strncpy (parms->ServerIP, DEF_SERVER_IP, CONF_STRING_LEN);
  strncpy (parms->ListenPort, DEF_LISTEN_PORT, CONF_STRING_LEN);
  strncpy (parms->ServerPort, DEF_SERVER_PORT, CONF_STRING_LEN);
  strncpy (parms->SerialPort, DEF_SERIAL_PORT, CONF_STRING_LEN);
  strncpy (parms->WebRoot, DEF_WEB_ROOT, CONF_STRING_LEN);
  strncpy (parms->WebPort, DEF_WEB_PORT, CONF_STRING_LEN);
  strncpy (parms->SerBaudRate, DEF_SER_BAUD_RATE, CONF_STRING_LEN);
  strncpy (parms->SmartRowsD, DEF_SMART_ROWS_D, CONF_STRING_LEN);
  strncpy (parms->SmartColumnsD, DEF_SMART_COLUMNS_D, CONF_STRING_LEN);
  strncpy (parms->SmartRowsNum, DEF_SMART_ROWS_NUM, CONF_STRING_LEN);
  strncpy (parms->SmartColumnsNum, DEF_SMART_COLUMNS_NUM, CONF_STRING_LEN);

}

/* trim: get rid of trailing and leading whitespace */
char *trim (char * s) {
  /* Initialise start, end pointers */
  char *s1 = s, *s2 = &s[strlen (s) - 1];
  /* Trim and delimit right side */
  while ( (isspace (*s2)) && (s2 >= s1) )
    s2--;
  *(s2+1) = '\0';
  /* Trim left side */
  while ( (isspace (*s1)) && (s1 < s2) )
    s1++;
  /* Copy finished string */
  strcpy (s, s1);
  return s;
}

/* parse external parameters file */
void parse_config (struct conf * parms) {
  char *s, buff[256];
  FILE *fp = fopen (CONFIG_FILE, "r");
  if (fp == NULL) { return; }

  /* Read next line */
  while ((s = fgets (buff, sizeof buff, fp)) != NULL){
    /* Skip blank lines and comments */
    if (buff[0] == '\n' || buff[0] == '#')
      continue;

    /* Parse name/value pair from line */
    char name[CONF_STRING_LEN], value[CONF_STRING_LEN];
    s = strtok (buff, "=");
    if (s==NULL)
      continue;
    else
      strncpy (name, s, CONF_STRING_LEN);
    s = strtok (NULL, "=");
    if (s==NULL)
      continue;
    else
      strncpy (value, s, CONF_STRING_LEN);
    trim (value);

    /* Copy into correct entry in parameters struct */
    if (strcmp(name, "ListenIP")==0)
      strncpy (parms->ListenIP, value, CONF_STRING_LEN);
    else if (strcmp(name, "ListenPort")==0)
      strncpy (parms->ListenPort, value, CONF_STRING_LEN);
    else if (strcmp(name, "SerialPort")==0)
      strncpy (parms->SerialPort, value, CONF_STRING_LEN);
    else if (strcmp(name, "WebRoot")==0)
      strncpy (parms->WebRoot, value, CONF_STRING_LEN);
    else if (strcmp(name, "WebPort")==0)
      strncpy (parms->WebPort, value, CONF_STRING_LEN);
    else if (strcmp(name, "ServerIP")==0)
      strncpy (parms->ServerIP, value, CONF_STRING_LEN);
    else if (strcmp(name, "ServerPort")==0)
      strncpy (parms->ServerPort, value, CONF_STRING_LEN);
    else if (strcmp(name, "ServerPort")==0)
      strncpy (parms->SmartRowsD, value, CONF_STRING_LEN);
    else if (strcmp(name, "SmartRowsD")==0)
      strncpy (parms->SmartColumnsD, value, CONF_STRING_LEN);
    else if (strcmp(name, "SmartColumnsD")==0)
      strncpy (parms->SmartRowsNum, value, CONF_STRING_LEN);
    else if (strcmp(name, "SmartColumnsNum")==0)
      strncpy (parms->SmartColumnsNum, value, CONF_STRING_LEN);
    else
      syslog (LOG_INFO,"WARNING: %s/%s: Unknown name/value pair!\n", name, value);
  }
  /* Close file */
  fclose (fp);
}
