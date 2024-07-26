#ifndef ALPHA_RESPONSE
#define ALPHA_RESPONSE

#include "http.h"

typedef enum ResponseType {
  RESPONSE_HTML = 1,
  RESPONSE_JSON = 2,
  RESPONSE_JSON_FILE = 3,
  RESPONSE_HTML_FILE = 4,
} ResponseType;

typedef struct {
  char *title;
  char *body;
} HtmlPayload;

typedef union ResponsePayload {
  HtmlPayload html;
  char *filePath;
  struct Json *jsonObject;
} ResponsePayload;

typedef struct Response {
  ResponseType type;
  StatusCode statusCode;
  ResponsePayload payload;
} Response;

void response_handler(int *client_fd, Response res);
void handle_response_with_html(int *client_fd, Response res);
void handle_response_with_json(int *client_fd, Response res);
void handle_response_with_html_file(int *client_fd, Response res);
void handle_response_with_json_file(int *client_fd, Response res);
void send_string_response(int *client_fd, StatusCode Status, char *title,
                          char *body);

#endif
