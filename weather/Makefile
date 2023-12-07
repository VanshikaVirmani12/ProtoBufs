# add xml json
all: client server

BUILD_DIR = build

CC = gcc
CFLAGS = -Wall -g -Wno-pointer-sign

LJANSSON = -ljansson
LXML = -I/opt/homebrew/opt/libxml2/lib -lxml2

lib_weather_data_def.so: weather_data_def.o
	$(CC) $(CFLAGS) -shared -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/protobuf.o: protobuf/weather_data_pb.c protobuf/weather_data_pb.h
	$(CC) $(CFLAGS) -c protobuf/weather_data_pb.c -o $@

$(BUILD_DIR)/json.o: json/weather_data_json.c json/weather_data_json.h
	$(CC) $(CFLAGS) -c json/weather_data_json.c -o $@

$(BUILD_DIR)/xml.o: xml/weather_data_xml.c xml/weather_data_xml.h
	$(CC) $(CFLAGS) -c xml/weather_data_xml.c -o $@

client: client.o weather_data_def.c $(BUILD_DIR)/protobuf.o $(BUILD_DIR)/json.o $(BUILD_DIR)/xml.o
	$(CC) $(CFLAGS) -o client $^ $(LJANSSON) $(LXML)

server: server.o weather_data_def.c $(BUILD_DIR)/protobuf.o $(BUILD_DIR)/json.o $(BUILD_DIR)/xml.o
	$(CC) $(CFLAGS) -o server $^ $(LJANSSON) $(LXML)

clean:
	rm -f *.o weather_data_def.o build/*.o client server
