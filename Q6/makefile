.PHONY: all clean

CC = g++
FLAGS = -g -std=c++11

MYNC_OBJ = mynetcat.o tcp_client.o tcp_server.o udp_server.o udp_client.o uds_server.o uds_client.o 
TTT_OBJ = ttt.o tttfunctions.o

all: mync ttt

mync: $(MYNC_OBJ)
	$(CC) $(FLAGS) -o $@ $^

ttt: $(TTT_OBJ)
	$(CC) $(FLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(FLAGS) -c $<

clean: 
	rm -f *.o mync ttt