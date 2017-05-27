/*
 * Select.hpp
 *
 *  Created on: Apr 9, 2017
 *      Author: valerius
 */

#ifndef SELECT_HPP_
#define SELECT_HPP_

#include "Socket.hpp"

/*struct SelectMultiplexer : Multiplexer {
private:
	char name [7];
	fd_set set;
	int fd;
	timeval timeout;
public:
  SelectMultiplexer(int fd = 0, timeval timeout = {1, 0}) : fd(fd), timeout(timeout){
	  strcpy(this->name, "select");
  }
  ~SelectMultiplexer() {
	  FD_CLR(fd, &set);
  }
};/**/



#endif /* SELECT_HPP_ */
