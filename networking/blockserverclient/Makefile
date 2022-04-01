CC=gcc
CXX=g++
CXXFLAGS=-g -Wall -Werror -O0 -Wextra
all: client server 

client: client.c
	$(CC) $(CXXFLAGS) -o client client.c

server: server.c
	$(CC) $(CXXFLAGS) -o server server.c

clean:
	rm client server	
