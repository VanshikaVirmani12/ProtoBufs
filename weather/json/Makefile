all: client server

CC = gcc
CFLAGS = -Wall -g

LDFLAGS = -ljansson

client: client.o weather_data_json.o
	$(CC) $(CFLAGS) -o client $^ $(LDFLAGS)

server: server.o weather_data_json.o
	$(CC) $(CFLAGS) -o server $^ $(LDFLAGS)

weather_data_json.o: weather_data_json.h weather_data_json.c
	$(CC) $(CFLAGS) -c weather_data_json.c

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o client server
