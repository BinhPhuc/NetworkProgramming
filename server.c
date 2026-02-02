#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
  struct addrinfo hints, *res;
  struct sockaddr_storage their_addr;
  int status;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC; // Allow IPv4 or IPv6
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE; // For wildcard IP address
  if ((status = getaddrinfo(NULL, "8080", &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    return -1;
  }
  struct addrinfo *rp;
  for (rp = res; rp != NULL; rp = rp->ai_next) {
    int sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sockfd == -1) {
      perror("socket");
      continue;
    }
    int yes = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == -1) {
      perror("bind");
      continue;
    }
    if (listen(sockfd, 10) == -1) {
      perror("listen");
      continue;
    }
    printf("Server listening on port 8080\n");
    // Accept a single connection for demonstration purposes
    int new_fd = accept(sockfd, (struct sockaddr *)&their_addr,
                        &(socklen_t){sizeof(their_addr)});
  }
  return 0;
}
