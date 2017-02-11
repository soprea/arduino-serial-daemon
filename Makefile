all:
	gcc `mysql_config --cflags` `mysql_config --libs` arduino-serial-daemon.c arduino-serial-lib-daemon.c config.c webchild.c mysql.c webserver.c mongoose.c -o arduino-serial-daemon -Wall -Wno-unused-function -pthread

clean:
	rm -f arduino-serial-daemon

#PROG = webserver
#include examples.mk
