#ifndef ALPHA_HTTP
#define ALPHA_HTTP

typedef enum { GET = 1, POST = 2 } HttpMethod;

typedef enum {
  OK = 200,
  NOT_FOUND = 404,
  INTERNAL_ERROR = 500,
} StatusCode;

#endif
