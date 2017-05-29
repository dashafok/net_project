/*
 * PceudoCode.hpp
 *
 *  Created on: 06.04.2017
 *      Author: valerius
 */

#ifndef PCEUDOCODE_HPP_
#define PCEUDOCODE_HPP_


#include <iostream>
#include <fstream>
#include <stdio.h>

#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <cmath>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <json/json.h>
#include <crypt.h>
#include <fcntl.h>

#define ERROR perror("ERROR"); std::cout << strerror(errno) << " in line " << __LINE__ << " in " << __FILE__ << std::endl; exit(EXIT_FAILURE);

void *web(void *arg);
void make_socket_non_blocking (int sfd);

struct thread_data {
  int sockfd;
};

struct Multiplexer {
  //constructors cannot be declared virtual
  virtual ~Multiplexer () = 0;
};

void *acceptConnBySelect(void *arg);
void acceptConnByEpoll(const int sockfd);

#endif /* PCEUDOCODE_HPP_ */
