#include "Socket.hpp"

#define DATA_SIZE 14
#define LISTEN_BACKLOG 5 //the maximum length to which the queue of pending connections for sockfd may grow

using namespace std;

int main(int argc, const char* argv[]) {

  if (argc < 7) {
    ERROR;
  }

  int port_num = -1;
  int domain = AF_INET; //AF_UNIX;
  int type = SOCK_STREAM; //(TCP) SOCK_DGRAM (UPD)
  int protocol = 0;
  int multip_sel = -1;

  int sockfd = socket (domain, type, protocol); // socket file descriptor
  if (sockfd == -1) {
    ERROR;
  }
  make_socket_non_blocking(sockfd);

  int opt = 0;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    ERROR;
  }

  sockaddr_in serv_addr;
  serv_addr.sin_family = domain;
  bzero((void*)&(serv_addr.sin_addr), sizeof(serv_addr.sin_addr));

  //==================================
  for (int i = 1; i < argc; i++) {
    if ((argv[i][0] == '-') && (argv[i][0] == '-')) {
      if (i+1 == argc) {
        ERROR;
      }
      if (strcmp(argv[i], "--listen") == 0) {
        inet_pton(domain, argv[i+1], &(serv_addr.sin_addr));
        //std::cout << "adress = " << argv[i+1] << std::endl;
      } else if (strcmp(argv[i], "--port") == 0) {
        port_num = atoi (argv[i+1]);
        serv_addr.sin_port = htons (port_num); //value in TCP/IP network byte order
        //std::cout << "port_num = " << port_num << std::endl;
        /*if (!port_num) {
          ERROR;
        }/**/
      } else if (strcmp(argv[i], "--multiplexer") == 0) {
        if (strcmp(argv[i+1], "select") == 0) {
          multip_sel = 1;
        } else if(strcmp(argv[i+1], "epoll") == 0) {
          multip_sel = 0;
        } else {
          ERROR;
        }
        //std::cout << "multiplexer = " << multip_sel << std::endl;
      } else {
        ERROR;
      }
    }
  }
  if (multip_sel == -1 || port_num == -1) {
    ERROR;
  }/**/
//==========================================
  /*ArgumentParser parser;
  parser.addArgument("--listen", 1, true);
  parser.addArgument("--port", 1, true);
  parser.addArgument("--multiplexer", 1, true);
  parser.parse(argc, argv);
/**/
  socklen_t addrlen = sizeof(serv_addr);
  if (bind (sockfd, (sockaddr*)&serv_addr, addrlen) == -1) {
    ERROR;
  }

  if (listen(sockfd, LISTEN_BACKLOG) == -1) {
    ERROR;
  }

  std::cout << "Start receiving connections ..." << std::endl;

  int pid = fork();
  if(pid < 0) {
    ERROR;
  }
  if (!pid) {
    umask(0);
    setsid();
    chdir("/");

    thread_data data = {sockfd};
    if (multip_sel) {
      acceptConnBySelect((void *)&data);
    } else {
      acceptConnByEpoll(sockfd);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
  } else {
    return 0;
  }

}
