/*
 * Epoll.hpp
 *
 *  Created on: Apr 9, 2017
 *      Author: valerius
 */

#ifndef EPOLL_HPP_
#define EPOLL_HPP_

#include "Socket.hpp"

struct EpollMultiplexer : Multiplexer{
    char name[7];
    epoll_event event;
    int fd;
    EpollMultiplexer () {
    	strcpy(name, "epoll");
    	fd = epoll_create(1);
    }
    void listenFd (int fd) {
    	event.data.fd = fd;
        epoll_ctl(this->fd, EPOLL_CTL_ADD, fd, &event);
    }
    ~EpollMultiplexer () {

    }
};



#endif /* EPOLL_HPP_ */
