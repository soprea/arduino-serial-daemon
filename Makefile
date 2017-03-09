all:
	gcc ardubot.c ardubot-lib.c config.c webchild.c -o ardubot -Wall -Wno-unused-function -pthread

clean:
	rm -f ardubot

install:
	cp ardubot.conf /etc/ardubot.conf
	
uninstall:
	rm -f /etc/ardubot.conf
#PROG = webserver
#include examples.mk
