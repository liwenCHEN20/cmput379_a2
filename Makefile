CC=gcc
CFLAGS=-m32 -g -pthread
LDFLAGS=-m32 -pthread
DEPS=server.h
OBJS=main.o responses.o logging.o server.o

all:	server_p server_f

server_f:	$(OBJS) server_f.o
	$(CC) -o $@ $(OBJS) server_f.o $(LDFLAGS)

server_p:	$(OBJS) server_p.o
	$(CC) -o $@ $(OBJS) server_p.o $(LDFLAGS)

%.o:	%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 
clean:
	rm -rf server_p server_f *.o
