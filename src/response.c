#include <assert.h>
#include <errno.h>
#include <stdio.h>

// Note: JACK_IMPLEMENTATION cannot be defined in other place
#define JACK_IMPLEMENTATION
#include "../exteral/jack/include/jack.h"

// LOG4C_IMPLEMENTATION already defined at 'alpha.c'
#include "../exteral/log4c/include/log4c.h"

#include "../include/alpha.h"
#include "../include/alpha/response.h"
#include "../include/alpha/templates.h"

// Implemented at 'request.c' file
char *join_strings(char *lhs, char *rhs);

void response_handler(int *client_fd, Response response) {
  switch (response.type) {
  case RESPONSE_HTML:
    handle_response_with_html(client_fd, response);
    break;
  case RESPONSE_JSON:
    handle_response_with_json(client_fd, response);
    break;
  case RESPONSE_HTML_FILE:
    handle_response_with_html_file(client_fd, response);
    break;
  case RESPONSE_JSON_FILE:
    handle_response_with_json_file(client_fd, response);
    break;
  }
}

void send_string_response(int *client_fd, StatusCode status_code, char *title,
                          char *text) {
  Response response = {
      .payload.html.title = title,
      .payload.html.body = text,
      .statusCode = status_code,
      .type = RESPONSE_HTML,
  };
  handle_response_with_html(client_fd, response);
}

void handle_response_with_html(int *client_fd, Response response) {
  usize page_title_len = strlen(response.payload.html.title);
  usize body_template_len = strlen(HTML_TEMPLATE);
  usize html_text_len = strlen(response.payload.html.body);
  dprintf(*client_fd, HTTP_HEADER_TEMPLATE, response.statusCode, "text/html",
          page_title_len + body_template_len + html_text_len);
  dprintf(*client_fd, HTML_TEMPLATE, response.payload.html.title,
          response.payload.html.body);
}

void handle_response_with_json(int *client_fd, Response response) {
  char *json_string = Json_Stringfy(*response.payload.jsonObject, 0);
  dprintf(*client_fd, HTTP_HEADER_TEMPLATE, response.statusCode,
          "application/json", strlen(json_string));
  dprintf(*client_fd, "%s", json_string);
}

void handle_response_with_html_file(int *client_fd, Response response) {
  FILE *file =
      fopen(join_strings(STATIC_FOLDER_PATH, response.payload.filePath), "r");

  if (!file) {
    Log(stderr, ERROR, "Couldn't respond with file %s: %s",
        response.payload.filePath, strerror(errno));
    send_string_response(client_fd, 500, "Internal ERROR",
                         "<h1>Internal Server ERROR</h1>");
    return;
  }

  fseek(file, 0, SEEK_END);
  usize file_len = ftell(file);
  fseek(file, 0, SEEK_SET);

  dprintf(*client_fd, HTTP_HEADER_TEMPLATE, response.statusCode, "text/html",
          file_len);

  usize read_len = 0;
  usize chunck_len = 512;
  char chunck[chunck_len];

  while (1) {
    read_len = fread(chunck, sizeof(char), chunck_len, file);
    if (read_len < 0) {
      Log(stderr, ERROR, "Couldn't read chunck from specified file %s\n",
          strerror(errno));
      send_string_response(client_fd, 500, "Internal ERROR",
                           "<h1>Internal Server ERROR</h1>");
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

void handle_response_with_json_file(int *client_fd, Response response) {
  FILE *file =
      fopen(join_strings(STATIC_FOLDER_PATH, response.payload.filePath), "r");

  if (!file) {
    Log(stderr, ERROR, "Couldn't respond with file %s: %s",
        response.payload.filePath, strerror(errno));
    send_string_response(client_fd, 500, "Internal ERROR",
                         "<h1>Internal Server ERROR</h1>");
    return;
  }

  fseek(file, 0, SEEK_END);
  usize file_len = ftell(file);
  fseek(file, 0, SEEK_SET);

  dprintf(*client_fd, HTTP_HEADER_TEMPLATE, response.statusCode,
          "application/json", file_len);

  usize read_len = 0;
  usize chunck_len = 512;
  char chunck[chunck_len];

  while (1) {
    read_len = fread(chunck, sizeof(char), chunck_len, file);
    if (read_len < 0) {
      Log(stderr, ERROR, "Couldn't read chunck from specified file %s\n",
          strerror(errno));
      send_string_response(client_fd, 500, "Internal ERROR",
                           "<h1>Internal Server ERROR</h1>");
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
