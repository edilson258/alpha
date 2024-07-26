#ifndef ALPHA_ROUTER
#define ALPHA_ROUTER

#include "common.h"
#include "request.h"
#include "response.h"

typedef Response (*AlphaRouteHandler)(Request);

typedef struct {
  char *_path;
  HttpMethod _method;
  AlphaRouteHandler _handler;
} Route;

typedef struct {
  usize _capacity;
  usize _routesCount;
  Route _routes[ROUTER_INIT_CAP];
} Router;

#endif
