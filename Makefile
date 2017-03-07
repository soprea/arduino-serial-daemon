all:
	gcc ardubot.c ardubot-lib.c config.c webchild.c -o ardubot -Wall -Wno-unused-function -pthread
	gcc joya.c -o joya -Wall -Wno-unused-function
	gcc webserver.c mongoose.c config.c -o webserver -Wall -Wno-unused-function

clean:
	rm -f ardubot
	rm -f joya
	rm -f webserver

install:
	cp ardubot.conf /etc/ardubot.conf
	
uninstall:
	rm -f /etc/ardubot.conf
#PROG = webserver
#include examples.mk
