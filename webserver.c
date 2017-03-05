#include "config.h"
#include "header.h"
#include "webserver.h"

struct device_settings {
  char setting1[100];
  char setting2[100];
};
char keypressed[10];

struct mg_serve_http_opts s_http_server_opts;
struct device_settings s_settings = {"value1", "value2"};
void sendKey(char *keypressed){
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    char *servIP = "127.0.0.1";                   /* Server IP address (dotted quad) */
    int value = 1;
    int echoServPort = 4000;  /* server port */

    /* Create a reliable, stream socket using TCP */
    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

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

void handle_save(struct mg_connection *nc, struct http_message *hm) {
  // Get form variables and store settings values
  mg_get_http_var(&hm->body, "setting1", s_settings.setting1,
                  sizeof(s_settings.setting1));
  mg_get_http_var(&hm->body, "setting2", s_settings.setting2,
                  sizeof(s_settings.setting2));
  mg_get_http_var(&hm->body, "key_pressed", keypressed,
                  sizeof(keypressed));
  sendKey(keypressed);
        FILE *fp = fopen("out2.txt","a"); /* Open file in append monde */
        fprintf(fp, "%s Read string: %s \n", s_settings.setting1, s_settings.setting2); /* Write output in file */
        fclose(fp);/* Close file */
 //           serialport_write(fd, keypressed); //I have to error check this

  // Send response
  mg_http_send_redirect(nc, 302, mg_mk_str("/"), mg_mk_str(NULL));
}

void handle_get_cpu_usage(struct mg_connection *nc) {
  // Generate random value, as an example of changing CPU usage
  // Getting real CPU usage depends on the OS.
  int cpu_usage = (double) rand() / RAND_MAX * 100.0;

  // Use chunked encoding in order to avoid calculating Content-Length
  mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");

  // Output JSON object which holds CPU usage data
  mg_printf_http_chunk(nc, "{ \"result\": %d }", cpu_usage);

  // Send empty chunk, the end of response
  mg_send_http_chunk(nc, "", 0);
}

void handle_ssi_call(struct mg_connection *nc, const char *param) {
  if (strcmp(param, "setting1") == 0) {
    mg_printf_html_escape(nc, "%s", s_settings.setting1);
  } else if (strcmp(param, "setting2") == 0) {
    mg_printf_html_escape(nc, "%s", s_settings.setting2);
  }
}

void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  struct http_message *hm = (struct http_message *) ev_data;

  switch (ev) {
    case MG_EV_HTTP_REQUEST:
      if (mg_vcmp(&hm->uri, "/save") == 0) {
        handle_save(nc, hm);
      } else if (mg_vcmp(&hm->uri, "/get_cpu_usage") == 0) {
        handle_get_cpu_usage(nc);
      } else {
        mg_serve_http(nc, hm, s_http_server_opts);  // Serve static content
      }
      break;
    case MG_EV_SSI_CALL:
      handle_ssi_call(nc, ev_data);
      break;
    default:
      break;
  }
}

void webserver() {
  struct mg_mgr mgr;
  struct mg_connection *nc;
  cs_stat_t st;

  struct conf parms;
  /* configuration file */
  init_parameters(&parms);
  parse_config(&parms);  
  
  const char *s_http_port = parms.WebPort;  
  
  mg_mgr_init(&mgr, NULL);
  nc = mg_bind(&mgr, s_http_port, ev_handler);
  if (nc == NULL) {
    syslog(LOG_INFO, "Cannot bind to %s\n", s_http_port);
    exit(1);
  }

  // Set up HTTP server parameters
  mg_set_protocol_http_websocket(nc);
  s_http_server_opts.document_root = parms.WebRoot;;  // Set up web root directory

  if (mg_stat(s_http_server_opts.document_root, &st) != 0) {
    syslog(LOG_INFO, "Cannot find web_root directory, exiting\n");
    exit(1);
  }

  syslog(LOG_INFO,"Starting web server on port %s\n", s_http_port);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  mg_mgr_free(&mgr);

//  return 0;
}
