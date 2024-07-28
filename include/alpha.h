#ifndef ALPHA
#define ALPHA

#include "alpha/common.h"
#include "alpha/router.h"

#define BACK_LOG 5120
#if !defined(STATIC_FOLDER_PATH)
#define STATIC_FOLDER_PATH "static/"
#endif

typedef struct {
  int _fileDescriptor;
  usize _port;
  usize _backLog;
  Router _router;
} AlphaApp;

AlphaApp Alpha_New(char *host, unsigned long port);
void Alpha_Get(AlphaApp *app, char *path, AlphaRouteHandler handler);
void Alpha_Run(AlphaApp *app);

#endif
