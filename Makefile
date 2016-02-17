CC=clang++
CFLAGS=-std=c++11 -pthread -Wall -Wextra -Werror -pedantic

socket.o: Socket.h Socket.cpp
	$(CC) $(CFLAGS) -o bin/socket.o

clean:
	rm -f bin/*

