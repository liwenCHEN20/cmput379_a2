CC=gcc
CFLAGS=-m32 -g
LDFLAGS=-m32
DEPS=server.h

all:	server_p server_f

server_f:	main.o responses.o server_f.o
	$(CC) -o $@ main.o responses.o server_f.o $(LDFLAGS)

server_p:	main.o responses.o server_p.o
	$(CC) -o $@ main.o responses.o server_p.o $(LDFLAGS)

%.o:	%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 
clean:
	rm -rf server_p server_f *.o
