PROG = smart

all:
	gcc $(PROG).c ../utils/config.c ../utils/utils.c -o $(PROG) -Wall -Wno-unused-function -pthread

clean:
	rm -rf *.gc* *.dSYM *.exe *.obj *.o a.out $(PROG)

install:
	cp ../ardubot.conf /etc/ardubot.conf

uninstall:
	rm -f /etc/ardubot.conf
