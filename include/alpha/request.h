#ifndef ALPHA_REQUEST
#define ALPHA_REQUEST

#include "http.h"

typedef struct {
  const char *path;
  HttpMethod method;
} Request;

void *RequestHandler(void *arg);

#endif
