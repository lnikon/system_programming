#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/fcntl.h>

#include "utility.h"

#define PORT "3490"
#define BACKLOG 10

void sigchld_handler(int s);

/**
 * Used to initialize the server.
 */
int init_server(addrinfo *hints, addrinfo *servinfo, int &sockfd,
                char s[INET6_ADDRSTRLEN], int size_s);

/**
 * Used to handle the incoming connections.
 */
void handle_connection(sockaddr_storage *their_addr, int sockfd,
                       char s[INET6_ADDRSTRLEN], int size_s);

/**
 * Used to make alread created socket non-blocking.
 */
void setnonblocking(int fd);

int main(void) {
  int sockfd;
  addrinfo hints;
  addrinfo *servinfo = nullptr;
  sockaddr_storage their_addr;
  char s[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  init_server(&hints, servinfo, sockfd, s, INET6_ADDRSTRLEN);

  printf("server: waiting for connections...\n");

  // handle_connection
  handle_connection(&their_addr, sockfd, s, INET6_ADDRSTRLEN);
  return 0;
}

void sigchld_handler(int /* s */) {
  int saved_errno = errno;
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
  errno = saved_errno;
}

int init_server(addrinfo *hints, addrinfo *servinfo, int &sockfd,
                char s[INET6_ADDRSTRLEN], int size_s) {
  int rv;
  if ((rv = getaddrinfo(NULL, PORT, hints, &servinfo)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  addrinfo *p = nullptr;
  for (p = servinfo; p != nullptr; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    int yes = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    break;
  }

  inet_ntop(p->ai_addr->sa_family,
            get_in_addr(static_cast<sockaddr *>(p->ai_addr)), s, size_s);
  printf("server: my address is %s\n", s);

  freeaddrinfo(servinfo);

  if (p == NULL) {
    fprintf(stderr, "server: failed to bind\n");
    exit(1);
  }

  if (listen(sockfd, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  struct sigaction sa;
  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

  return 0;
}
#define MAX_EVENTS 1024
void handle_connection(sockaddr_storage *their_addr, int sockfd,
                       char s[INET6_ADDRSTRLEN], int s_size) {
  socklen_t sin_size = sizeof(sockaddr_storage);
  struct epoll_event ev, events[MAX_EVENTS];
  int nfds, epollfd;

  epollfd = epoll_create1(0);
  if (epollfd == -1) {
    perror("epoll_create1");
    exit(EXIT_FAILURE);
  }

  ev.events = EPOLLIN;
  ev.data.fd = sockfd;
  if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &ev) == -1) {
    perror("epoll_ctl: sockfd");
    exit(EXIT_FAILURE);
  }

  for (;;) {
    nfds = epoll_wait(epollfd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_wait");
      exit(EXIT_FAILURE);
    }

    for (int n = 0; n < nfds; ++n) {
      if (events[n].data.fd == sockfd) {
        int conn_sock =
            accept(sockfd, reinterpret_cast<sockaddr *>(their_addr), &sin_size);
        if (conn_sock == -1) {
          perror("accept");
          exit(EXIT_FAILURE);
        }
        setnonblocking(conn_sock);

        inet_ntop(their_addr->ss_family,
                  get_in_addr(reinterpret_cast<sockaddr *>(their_addr)), s,
                  s_size);
        printf("server: got connection from %s\n", s);

        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = conn_sock;
        if (epoll_ctl(epollfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1) {
          perror("epoll_ctl: conn_sock");
          exit(EXIT_FAILURE);
        } else {
          if (send(conn_sock, "Hello, world!", 13, 0) == -1) {
            perror("send");
          }
          close(conn_sock);
        }
      }
    }
  }

  //  while (1) {
  //    socklen_t sin_size = sizeof(sockaddr_storage);
  //    int newfd =
  //        accept(sockfd, reinterpret_cast<sockaddr *>(their_addr), &sin_size);
  //    if (newfd == -1) {
  //      perror("accept");
  //      continue;
  //    }
  //
  //    inet_ntop(their_addr->ss_family,
  //              get_in_addr(reinterpret_cast<sockaddr *>(their_addr)), s,
  //              size_s);
  //    printf("server: got connection from %s\n", s);
  //
  //    if (!fork()) {
  //      close(sockfd);
  //      if (send(newfd, "Hello, world!", 13, 0) == -1) {
  //        perror("send");
  //      }
  //      close(newfd);
  //      exit(0);
  //    }
  //    close(newfd);
  //  }
}

void setnonblocking(int fd) {
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
}
