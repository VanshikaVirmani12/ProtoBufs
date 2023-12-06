#include <stdio.h>
#include <stdlib.h>

#include "weather_data.pb.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s <port> <ip address>\n", argv[0]);
    return 1;
  }

  int port = atoi(argv[1]);
  char *ip_address = argv[2];
}
