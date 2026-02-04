#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int status;
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;
  if ((status = getaddrinfo("127.0.0.1", "8080", &hints, &res) != 0)) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    return -1;
  }
  struct addrinfo *p;
  char ipaddr[INET6_ADDRSTRLEN];
  int sockfd;
  for (p = res; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("socket");
      continue;
    }
    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("connect");
      continue;
    }
    break; // connected successfully
  }
  freeaddrinfo(res);
  if (p == NULL) {
    fprintf(stderr, "Could not connect\n");
    return -1;
  }
  char *msg = "Hey server! How are you today!";
  int bytes_sent = send(sockfd, msg, strlen(msg), 0);
  printf("Sent %d bytes to server\n", bytes_sent);
  return 0;
}
