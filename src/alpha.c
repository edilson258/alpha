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

#include "../external/cthings/include/cthings.h"
#include "../include/alpha.h"
#include "../include/templates.h"

typedef struct {
  int m_Fd;
  struct sockaddr_in m_Addr;
} Client;

typedef struct {
  Client client;
  AlphaApp app;
} requestDTO;

char *fread_line(FILE **fp);
void *request_handler_bridge(void *arg);
void handle_request(requestDTO *payload);
char *fchop_while(FILE **fp, char stop_char);
void send_error(int *client_fd, char *reason);
HttpMethod fextract_request_method(FILE **fp);
int init_tcp_socket(char *Host, usize Port, usize BackLog);
Route *match_route(Router *r, char *path, HttpMethod method);
void handle_request_get(requestDTO *payload, FILE **client_fp, char *path);
char *join_strings(char *lhs, char *rhs);
void handle_response(int *client_fd, Response res);
void handle_response_with_str(int *client_fd, Response res);
void handle_response_with_html_file(int *client_fd, Response res);
void handle_response_with_json(int *client_fd, Response res);

Router Alpha_Router_New() {
  Router router;
  router.m_RoutesCount = 0;
  router.m_Capacity = ROUTER_INIT_CAP;
  return router;
}

AlphaApp Alpha_New(char *Host, usize Port, usize BackLog) {
  AlphaApp app = malloc(sizeof(__Alpha__));
  app->m_Router = Alpha_Router_New();
  app->m_BackLog = BackLog;
  app->m_Port = Port;
  // TODO: check if is -1
  app->m_Fd = init_tcp_socket(Host, Port, BackLog);
  return app;
};

void Alpha_Get(AlphaApp app, char *path, AlphaRouteHandler handler) {
  Route route = {
      .m_Path = path,
      .m_Method = GET,
      .m_Handler = handler,
  };
  app->m_Router.m_Routes[app->m_Router.m_RoutesCount++] = route;
}

void Alpha_Run(AlphaApp app) {
  printf("[INFO]: Http server listening on port %lu\n", app->m_Port);

  pthread_t threads[app->m_BackLog];
  unsigned int threads_count = 0;

  while (1) {
    struct sockaddr_in client_addr;
    usize client_addr_len = sizeof(client_addr);
    const int client_fd = accept(app->m_Fd, (struct sockaddr *)&client_addr,
                                 (socklen_t *)&client_addr_len);

    // printf("%s %d\n",
    // inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

    if (client_fd == -1) {
      Log(stderr, ERROR, "Couldn't Accept conn: %s\n", strerror(errno));
      continue;
    }

    Client client = {.m_Addr = client_addr, .m_Fd = client_fd};
    requestDTO *payload = malloc(sizeof(requestDTO));
    payload->app = app;
    payload->client = client;

    pthread_t thread;
    pthread_create(&thread, NULL, request_handler_bridge, payload);
    threads[threads_count++] = thread;
  }

  for (unsigned int i = 0; i < threads_count; ++i) {
    pthread_join(threads[i], NULL);
  }
};

void *request_handler_bridge(void *arg) {
  requestDTO *payload = (requestDTO *)arg;
  handle_request(payload);
  return NULL;
}

void handle_request(requestDTO *payload) {
  FILE *client_fp = fdopen(payload->client.m_Fd, "r");
  if (!client_fp) {
    Log(stderr, ERROR, "Couldn't open client fd: %s\n", strerror(errno));
    send_error(&payload->client.m_Fd, "Internal Server Error");
    goto difer;
  }
  HttpMethod method = fextract_request_method(&client_fp);
  if (method == -1) {
    send_error(&payload->client.m_Fd, "Unexpected Http Method");
    goto difer;
  }
  char *path = fchop_while(&client_fp, ' ');
  switch (method) {
  case GET:
    handle_request_get(payload, &client_fp, path);
    break;
  case POST:
    send_error(&payload->client.m_Fd, "POST requests are not supported yet");
    break;
  }
  free(path);
difer:
  fclose(client_fp);
  close(payload->client.m_Fd);
}

void handle_request_get(requestDTO *payload, FILE **client_fp, char *path) {
  const Route *r = match_route(&payload->app->m_Router, path, GET);
  if (!r) {
    send_error(&payload->client.m_Fd, "404 path not found");
    Log(stdout, INFO, "GET %s %s", path, STATUS_CODE(NotFound));
    return;
  }
  const Request req = {
      .m_Method = GET,
      .m_Path = path,
  };
  const Response res = r->m_Handler(req);
  handle_response(&payload->client.m_Fd, res);
  Log(stdout, INFO, "GET %s %s", path, STATUS_CODE(res.m_Status));
}

void handle_response(int *client_fd, Response res) {
  switch (res.m_Type) {
  case STR:
    handle_response_with_str(client_fd, res);
    break;
  case FILE_:
    handle_response_with_html_file(client_fd, res);
    break;
  case JSON:
    handle_response_with_json(client_fd, res);
    break;
  }
}

Route *match_route(Router *router, char *path, HttpMethod method) {
  for (usize i = 0; i < router->m_RoutesCount; ++i) {
    Route *r = &router->m_Routes[i];
    if (strcmp(path, r->m_Path) == 0 && method == r->m_Method) {
      return r;
    }
  }
  return NULL;
}

HttpMethod fextract_request_method(FILE **client_fp) {
  char *provided_method_buf = fchop_while(client_fp, ' ');
  HttpMethod method;
  if (strcmp("GET", provided_method_buf) == 0) {
    method = GET;
  } else if (strcmp("POST", provided_method_buf)) {
    method = POST;
  } else {
    method = -1;
  }
  free(provided_method_buf);
  return method;
}

char *fchop_while(FILE **fp, char stop_char) {
  int c;
  char *buf = NULL;
  usize buf_len = 0;
  FILE *buf_stream = open_memstream(&buf, &buf_len);
  while ((c = fgetc(*fp)) != EOF) {
    if (c == stop_char)
      break;
    fputc(c, buf_stream);
  }
  fclose(buf_stream);
  return buf;
}

void send_error(int *client_fd, char *error_msg) {
  char *page_title = "Something went wrong";
  usize page_title_len = strlen(page_title);
  usize body_len = strlen(HTTP_BODY_TEMPLATE);
  usize error_msg_len = strlen(error_msg);
  dprintf(*client_fd, HTTP_HEADER_TEMPLATE, 400, "text/html",
          body_len + error_msg_len + page_title_len);
  dprintf(*client_fd, HTTP_BODY_TEMPLATE, page_title, error_msg);
}

int init_tcp_socket(char *Host, usize Port, usize BackLog) {
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
  if (listen(fd, BackLog) == -1) {
    Log(stderr, ERROR, "Couldn't Listen: %s\n", strerror(errno));
    return -1;
  }
  return fd;
}

char *join_strings(char *lhs, char *rhs) {
  usize lhs_len = strlen(lhs);
  usize rhs_len = strlen(rhs);
  char *buf = malloc(sizeof(char) * (lhs_len + rhs_len + 1));
  strncpy(buf, lhs, lhs_len);
  strncpy(buf + lhs_len, rhs, rhs_len);
  return buf;
}

void handle_response_with_str(int *client_fd, Response res) {
  char *page_title = "Well Done";
  usize page_title_len = strlen(page_title);
  usize body_len = strlen(HTTP_BODY_TEMPLATE);
  usize message_len = strlen(res.payload.m_String);
  dprintf(*client_fd, HTTP_HEADER_TEMPLATE, res.m_Status, "text/html",
          body_len + message_len + page_title_len);
  dprintf(*client_fd, HTTP_BODY_TEMPLATE, page_title, res.payload.m_String);
}

void handle_response_with_json(int *client_fd, Response res) {
  char *json_string = Json_Stringfy(res.payload.m_Json, 2);
  dprintf(*client_fd, HTTP_HEADER_TEMPLATE, res.m_Status, "Application/json",
          sizeof(char) * strlen(json_string));
  dprintf(*client_fd, "%s", json_string);
}

void handle_response_with_html_file(int *client_fd, Response res) {
  FILE *file =
      fopen(join_strings(STATIC_FOLDER_PATH, res.payload.m_FilePath), "r");
  if (!file) {
    Log(stderr, ERROR, "Couldn't respond with file %s: %s",
        res.payload.m_FilePath, strerror(errno));
    send_error(client_fd, "Internal Server ERROR");
    return;
  }
  fseek(file, 0, SEEK_END);
  usize file_len = ftell(file);
  fseek(file, 0, SEEK_SET);
  dprintf(*client_fd, HTTP_HEADER_TEMPLATE, 200, "text/html", file_len);

  // read and write in chuncks
  usize chunck_len = 512;
  char chunck[chunck_len];
  usize read_len = 0;

  while (1) {
    read_len = fread(chunck, sizeof(char), chunck_len, file);
    if (read_len < 0) {
      Log(stderr, ERROR, "Couldn't read chunck from specified file %s\n",
          strerror(errno));
      send_error(client_fd, "Internal Server ERROR");
      fclose(file);
      return;
    }
    dprintf(*client_fd, "%s", chunck);
    if (read_len < chunck_len) {
      break;
    }
  }
  fclose(file);
}
