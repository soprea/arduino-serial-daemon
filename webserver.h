#ifndef WEBSERVER_H_INCLUDED
#define WEBSERVER_H_INCLUDED
void webserver();
void handle_save(struct mg_connection *nc, struct http_message *hm);
void handle_get_cpu_usage(struct mg_connection *nc);
void handle_ssi_call(struct mg_connection *nc, const char *param);
void ev_handler(struct mg_connection *nc, int ev, void *ev_data);
#endif

