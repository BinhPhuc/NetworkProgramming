#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
  int status;
  struct addrinfo hints, *res;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;
  if ((status = getaddrinfo("youtube.com", "80", &hints, &res) != 0)) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    return -1;
  }
  struct addrinfo *p;
  char ipaddr[INET6_ADDRSTRLEN];
  for (p = res; p != NULL; p = p->ai_next) {
    char *ipver;
    void *addr;
    if (p->ai_family == AF_INET) {
      ipver = "IPv4";
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
    } else {
      ipver = "IPv6";
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
    }
    inet_ntop(p->ai_family, addr, ipaddr, INET6_ADDRSTRLEN);
    printf("%s: %s\n", ipver, ipaddr);
    int sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("socket");
      continue;
    }
    int con = connect(sockfd, p->ai_addr, p->ai_addrlen);
  }
  // printf("Finished resolving youtube.com\n");
  freeaddrinfo(res);
  return 0;
}
