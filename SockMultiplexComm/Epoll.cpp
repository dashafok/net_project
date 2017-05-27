#include "Socket.hpp"
#include "Epoll.hpp"

#define MAXEVENTS 64

void acceptConnByEpoll(int sockfd){
  int epoll_fd = epoll_create(1);
  if (epoll_fd == -1) {
    ERROR;
  }
  epoll_event event;
  event.data.fd = sockfd;
  event.events = EPOLLIN | EPOLLET;
  if (epoll_ctl( epoll_fd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
    ERROR;
  }

  epoll_event *events = new epoll_event[MAXEVENTS];

  while (true) {
    int nevs = 0;
    nevs = epoll_wait(epoll_fd, events, MAXEVENTS, -1);

    for (int i = 0; i < nevs; i++) {
      //check some events for errors?
      if (sockfd == events[i].data.fd) {
        sockaddr_in cli_addr;
        socklen_t addrlen;
        int accsockfd = accept (sockfd, (sockaddr*)&cli_addr, &addrlen);
        if (accsockfd == -1) {
          ERROR;
        }
        make_socket_non_blocking(accsockfd);


        event.data.fd = accsockfd;
        event.events = EPOLLIN | EPOLLET;
        if (epoll_ctl (epoll_fd, EPOLL_CTL_ADD, accsockfd, &event) == -1) {
          ERROR;
        }

      } else {
        pthread_t thread;
        thread_data data = {events[i].data.fd};
        if (pthread_create(&thread, NULL, web, (void *)&data)) {
          ERROR;
        }
        if (epoll_ctl (epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL) == -1) {
          ERROR;
        }
      }
    }

  }

//там где select делаем epoll_wait();
  //edge triggered - вот это нам вроде как
}
