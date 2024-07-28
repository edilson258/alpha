#include <arpa/inet.h>
#include <errno.h>
#include <linux/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/endian.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define LOG4C_IMPLEMENTATION
#include "../exteral/log4c/include/log4c.h"

#include "../include/alpha.h"
#include "../include/alpha/request.h"
#include "../include/alpha/request_dto.h"

int init_tcp_socket(char *Host, usize Port);

Router Alpha_Router_New() {
  Router router;
  router._routesCount = 0;
  router._capacity = ROUTER_INIT_CAP;
  return router;
}

AlphaApp Alpha_New(char *Host, usize Port) {
  // TODO: validate args
  AlphaApp app;
  app._router = Alpha_Router_New();
  app._backLog = BACK_LOG;
  app._port = Port;
  app._fileDescriptor = init_tcp_socket(Host, Port);
  return app;
}

void Alpha_Get(AlphaApp *app, char *path, AlphaRouteHandler handler) {
  // TODO: validate args
  Route route = {
      ._handler = handler,
      ._method = GET,
      ._path = path,
  };
  app->_router._routes[app->_router._routesCount++] = route;
}

// printf("%s %d\n",
// inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

void Alpha_Run(AlphaApp *app) {
  while (1) {
    struct sockaddr_in client_addr;
    usize client_addr_len = sizeof(client_addr);
    const int client_fd =
        accept(app->_fileDescriptor, (struct sockaddr *)&client_addr,
               (socklen_t *)&client_addr_len);

    if (client_fd == -1) {
      Log(stderr, ERROR, "Couldn't Accept conn: %s\n", strerror(errno));
      continue;
    }

    Client client = {.address = client_addr, .file_descriptor = client_fd};
    RequestDTO *payload = malloc(sizeof(RequestDTO));
    payload->app = app;
    payload->client = client;

    pthread_t thread;
    pthread_create(&thread, NULL, RequestHandler, payload);
  }
}

int init_tcp_socket(char *Host, usize Port) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    Log(stderr, ERROR, "Couldn't create server: %s\n", strerror(errno));
    return -1;
  }
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &(int){1},
                 sizeof(int)) == -1) {
    Log(stderr, ERROR, "Couldn't set to reuse Addr: %s\n", strerror(errno));
    return -1;
  }
  struct sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  saddr.sin_port = htons(Port);
  saddr.sin_addr.s_addr = inet_addr(Host);
  socklen_t saddr_len = sizeof(saddr);
  if (bind(fd, (struct sockaddr *)&saddr, saddr_len) == -1) {
    Log(stderr, ERROR, "Couldn't Bind: %s\n", strerror(errno));
    return -1;
  }
  if (listen(fd, BACK_LOG) == -1) {
    Log(stderr, ERROR, "Couldn't Listen: %s\n", strerror(errno));
    return -1;
  }
  return fd;
}
