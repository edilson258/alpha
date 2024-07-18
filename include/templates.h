#ifndef ALPHA_HTTP_TEMPLATES
#define ALPHA_HTTP_TEMPLATES

#define HTTP_HEADER_TEMPLATE                                                   \
  "HTTP/1.1 %d\r\n"                                                            \
  "Content-Type: %s\r\n"                                                       \
  "Content-Length: %lu\r\n"                                                    \
  "\r\n"

#define HTTP_BODY_TEMPLATE                                                     \
  "<!Doctype html5>"                                                           \
  "<html>"                                                                     \
  "<head>"                                                                     \
  "<title>%s</title>"                                                          \
  "<meta name=\"viewport\" "                                                   \
  "content=\"width=device-width, initial-scale=1\"/>"                          \
  "</head>"                                                                    \
  "<body>"                                                                     \
  "%s"                                                                         \
  "</body>"                                                                    \
  "</html>"

#endif
