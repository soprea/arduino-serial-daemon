#include "header.h"
#include "config.h"

#define CONFIG_FILE             "/etc/ardubot.conf"
#define CONF_STRING_LEN         100
#define DEF_SERVER_IP           "127.0.0.1"
#define DEF_LISTEN_IP           "127.0.0.1"
#define DEF_SERVER_PORT         "4000"
#define DEF_LISTEN_PORT         "4000"
#define DEF_SERIAL_PORT         "/dev/ttyACM0"
#define MAXLINE                 3000
#define DEF_WEB_ROOT            "/root/ardubot/web_root/"
#define DEF_WEB_PORT            "6080"


/* initialise data to default values */
void init_parameters (struct conf * parms) {
  strncpy (parms->ListenIP, DEF_LISTEN_IP, CONF_STRING_LEN);
  strncpy (parms->ServerIP, DEF_SERVER_IP, CONF_STRING_LEN);  
  strncpy (parms->ListenPort, DEF_LISTEN_PORT, CONF_STRING_LEN);
  strncpy (parms->ServerPort, DEF_SERVER_PORT, CONF_STRING_LEN);  
  strncpy (parms->SerialPort, DEF_SERIAL_PORT, CONF_STRING_LEN);
  strncpy (parms->WebRoot, DEF_WEB_ROOT, CONF_STRING_LEN);
  strncpy (parms->WebPort, DEF_WEB_PORT, CONF_STRING_LEN);
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
    else
      syslog (LOG_INFO,"WARNING: %s/%s: Unknown name/value pair!\n", name, value);
  }
  /* Close file */
  fclose (fp);
}
