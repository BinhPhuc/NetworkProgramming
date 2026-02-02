#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

int main() {
  struct sockaddr_in ina;
  int inet = inet_pton(AF_INET, "127.0.0.1", &ina.sin_addr);
  if (inet < 0) {
    fprintf(stderr, "inet_pton failed\n");
    return 1;
  }
  // char ip4[INET_ADDRSTRLEN];
  char *ip4;
  inet_ntop(AF_INET, &ina.sin_addr, ip4, INET_ADDRSTRLEN);
  printf("IPv4 address: %s\n", ip4);
  return 0;
}
