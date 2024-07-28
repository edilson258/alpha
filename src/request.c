#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include "../exteral/log4c/include/log4c.h"

#include "../include/alpha/request.h"
#include "../include/alpha/request_dto.h"
#include "../include/alpha/response.h"

#define STATUS_CODE(code)                                                      \
  ((code) == 200   ? "\033[0;32m200\033[0m"                                    \
   : (code) == 404 ? "\033[0;33m404\033[0m"                                    \
   : (code) == 500 ? "\033[0;33m500\033[0m"                                    \
                   : "Unknown Status Code")

// Helpers
char *fread_line(FILE *fp);
char *fchop_while(FILE *fp, char stop_char);
HttpMethod fextract_request_method(FILE *fp);

void handle_request(RequestDTO *payload);
void handle_request_get(RequestDTO *payload, FILE *client_fp, char *path);

// Called on each accepted connetion
void *RequestHandler(void *arg) {
  RequestDTO *payload = (RequestDTO *)arg;
  handle_request(payload);
  return NULL;
}

void handle_request(RequestDTO *payload) {
  FILE *client_fp = fdopen(payload->client.file_descriptor, "r");
  if (!client_fp) {
    Log(stderr, ERROR, "Couldn't open client fd: %s\n", strerror(errno));
    goto difer;
  }
  HttpMethod method = fextract_request_method(client_fp);
  if (method == -1) {
    send_string_response(&payload->client.file_descriptor, 400,
                         "Unexpected Http Method", "Unexpected Http Method");
    goto difer;
  }
  char *path = fchop_while(client_fp, ' ');
  switch (method) {
  case GET:
    handle_request_get(payload, client_fp, path);
    break;
  case POST:
    send_string_response(&payload->client.file_descriptor, 400,
                         "POST requests are not supported yet",
                         "POST requests are not supported yet");
    break;
  }
  free(path);
difer:
  fclose(client_fp);
  close(payload->client.file_descriptor);
  free(payload);
}

Route *match_route(Router *router, char *path, HttpMethod method) {
  for (usize i = 0; i < router->_routesCount; ++i) {
    Route *r = &router->_routes[i];
    if (strcmp(path, r->_path) == 0 && method == r->_method) {
      return r;
    }
  }
  return NULL;
}

void handle_request_get(RequestDTO *payload, FILE *client_fp, char *path) {
  const Route *route = match_route(&payload->app->_router, path, GET);
  if (!route) {
    send_string_response(&payload->client.file_descriptor, 400,
                         "404 path not found", "404 path not found");
    Log(stdout, INFO, "GET %s %s", path, STATUS_CODE(NOT_FOUND));
  } else {
    const Request request = {
        .method = GET,
        .path = path,
    };
    const Response response = route->_handler(request);
    response_handler(&payload->client.file_descriptor, response);
    Log(stdout, INFO, "GET %s %s", path, STATUS_CODE(response.statusCode));
  }
}

HttpMethod fextract_request_method(FILE *client_fp) {
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

char *fchop_while(FILE *fp, char stop_char) {
  int c;
  char *buf = NULL;
  usize buf_len = 0;
  FILE *buf_stream = open_memstream(&buf, &buf_len);
  while ((c = fgetc(fp)) != EOF) {
    if (c == stop_char)
      break;
    fputc(c, buf_stream);
  }
  fclose(buf_stream);
  return buf;
}
