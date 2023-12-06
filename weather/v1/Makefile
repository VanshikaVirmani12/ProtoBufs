all: client server

CC = gcc
CFLAGS = -Wall -g

client: client.o weather_data.pb.o
	$(CC) $(CFLAGS) -o client $^

server: server.o weather_data.pb.o
	$(CC) $(CFLAGS) -o server $^

weather_data_pb.o: weather_data.pb.h weather_data.pb.c

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o client server
