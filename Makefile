CC=gcc
CFLAGS=-m32 -g
LDFLAGS=-m32
DEPS=server.h
OBJS=main.o responses.o logging.o

all:	server_p server_f

server_f:	$(OBJS) server_f.o
	$(CC) -o $@ $(OBJS) server_f.o $(LDFLAGS)

server_p:	$(OBJS) server.o
	$(CC) -o $@ $(OBJS) server.o $(LDFLAGS)

%.o:	%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 
clean:
	rm -rf server_p server_f *.o
