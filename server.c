#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

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
    return EXIT_FAILURE;
  }
  struct addrinfo *rp;
  int sockfd;
  for (rp = res; rp != NULL; rp = rp->ai_next) {
    sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sockfd == -1) {
      perror("socket");
      continue;
    }
    int yes = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == -1) {
      perror("bind");
      close(sockfd);
      continue;
    }
    break; // binded successfully
  }
  freeaddrinfo(res);
  if (rp == NULL) {
    fprintf(stderr, "Could not bind\n");
    close(sockfd);
    return EXIT_FAILURE;
  }
  if (listen(sockfd, 10) == -1) {
    perror("listen");
    return EXIT_FAILURE;
  }
  printf("Server listening on port 8080\n");
  char buf[1024];
  while (1) {
    socklen_t addr_size = sizeof(their_addr);
    int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
    if (new_fd == -1) {
      perror("accept");
      continue;
    }
    printf("Accepted a new connection\n");
    // Here you can handle the new connection (new_fd)
    int numbytes = recv(new_fd, buf, sizeof(buf) - 1, 0);
    if (numbytes > 0) {
      buf[numbytes] = '\0';
      printf("Received: %s\n", buf);
    }
    close(new_fd); // Close the connection after handling
  }
  close(sockfd);
  return 0;
}
