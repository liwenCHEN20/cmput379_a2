CC=gcc
CFLAGS=-m32
DEPS=server.h

all:	server_p server_f

server_f:	main.o server_f.o

server_p:	main.o server_p.o

%.o:	%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 
clean:
	rm -rf server_p server_f *.o
