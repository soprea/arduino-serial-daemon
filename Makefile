all:
	gcc -v `mysql_config --cflags` `mysql_config --libs` arduino-serial-daemon.c arduino-serial-lib-daemon.c config.c webchild.c mysql.c -o arduino-serial-daemon -Wall

clean:
	rm -f arduino-serial-daemon
