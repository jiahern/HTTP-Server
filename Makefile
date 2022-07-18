server: server.o utils.o
	gcc -Wall -o server server.o utils.o -lpthread

server.o: server.c utils.h
	gcc -Wall -c server.c

utils.o: utils.c utils.h
	gcc -Wall -c utils.c

clean:
	rm -f *.o server