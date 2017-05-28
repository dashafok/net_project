#include "Select.hpp"

void acceptConnBySelect(const int sockfd) {
  fd_set rfds, afds;
  FD_ZERO(&afds);
  FD_ZERO(&rfds);
  //struct timeval timeout = {1, 0};//sec, usec
  int nfds = 0;

  FD_SET(sockfd, &afds);

  int i = 0;
  while (++i) {
    memcpy(&rfds,&afds,sizeof(rfds));
    nfds = select(FD_SETSIZE, &rfds, NULL, NULL, NULL);
    if (nfds < 0) {
      ERROR;
    }

    if (FD_ISSET(sockfd, &rfds)) {
      struct sockaddr_in cli_addr;
      socklen_t addrlen = sizeof(cli_addr);
      //std::cout << "TUT" << std::endl;
      int accsockfd = accept (sockfd, (struct sockaddr*)&cli_addr, &addrlen);
      if (accsockfd == -1) {
        ERROR;
      }
      FD_SET(accsockfd, &afds);

    }
    for (int fd = 0; fd < FD_SETSIZE; fd ++) {
      if (FD_ISSET (fd, &rfds) && fd != sockfd) {
        pthread_t thread;
        thread_data data = {fd};
        if (pthread_create(&thread, NULL, web, (void *)&data)) {
          ERROR;
        }
        FD_CLR(fd, &afds);
      }
    }

  }

}


