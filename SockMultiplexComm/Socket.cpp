#include "Socket.hpp"

void *web(void *arg) {
	thread_data data = *(thread_data *)arg;
    /*char method[8];
    char uri[16];
    char http[16];
    std::cout << "< ";
    std::cin >> method >> uri >> http;

    {
        char post[] = "POST";
        char hash[] = "/hash";
        char http_ver[] = "HTTP/1.0";
        if ((strcmp(method, post) != 0) || (strcmp(uri, hash) != 0) || (strcmp(http, http_ver) != 0)) {
            std::cout << "> HTTP/1.0 409 Conflict" << std::endl;
            pthread_exit(NULL);
        }
    }
    std::cout << "< " << std::endl;

    std::cout << "< ";/**/
    char buffer[256];
    /*for (int i = 0; ; i++) {
    	buffer[i] = getchar();
    	if (buffer[i] == '}'){
    	    int temp = i+1;
    		buffer[temp] = 0;
    		break;
    	}
    	i++;
    }/**/
    recv(data.sockfd, buffer, sizeof(buffer),0);
    //std::cout << buffer << std::endl;
    char buf_json[1024];
    for (int i = strlen(buffer) - 1; i >= 0; i--) {
      if (buffer[i] == '{') {
        strcpy(buf_json, &(buffer[i]));
      }
    }
    //std::cout << std::endl;/**/
    //strcpy (buffer, "{\"str\": \"random string\", \"rounds\": 2}");

    json_object * jobj = json_tokener_parse(buf_json);
    json_object * jobj_str;
    json_object * jobj_rou;
    if (!json_object_object_get_ex(jobj, "str", &jobj_str)) {
      ERROR;
      pthread_exit(NULL);
    }
    if (!json_object_object_get_ex(jobj, "rounds", &jobj_rou)) {
    	ERROR;
        pthread_exit(NULL);
    }

    char *hash = 0;
    char salt[256];
    sprintf(salt, "$6$%d", json_object_get_int(jobj_rou));
    hash = crypt (json_object_get_string(jobj_str), salt);
    if (!hash) {
      ERROR;
      pthread_exit(NULL);
    }

    char str[1024];
    //sprintf(str, "{\"str\": \"%s\", \"rounds\": %d}", json_object_get_string(jobj_str), json_object_get_int(jobj_rou));
    sprintf(str, "HTTP/1.0 200 OK\t\nServer: bzz\t\nContent-Type: application/json\t\nContent-length: %d\t\nConnection: close\t\n\t\n%s\t\n", strlen(hash), hash);
    send(data.sockfd, str, sizeof(str),0);
/*
    std::cout << "> HTTP/1.0 200 OK\n\t";
    std::cout << "> Server: bzz\n\t";
    std::cout << "> Content-Type: application/json\n\t";
    std::cout << "> Content-length: " << strlen(hash) << "\n\t";
    std::cout << "> Connection: close\n\t";
    std::cout << "> " << "\n\t";
    std::cout << "> " << hash << "\n\t";/**/

    pthread_exit(NULL);
}

void make_socket_non_blocking (int sfd) {
  int flags, s;

  flags = fcntl (sfd, F_GETFL, 0);
  if (flags == -1) {
    ERROR;
  }

  flags |= O_NONBLOCK;
  s = fcntl (sfd, F_SETFL, flags);
  if (s == -1) {
    ERROR;
  }
}

