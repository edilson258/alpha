#ifndef ALPHA_HTTP
#define ALPHA_HTTP

#ifdef ALPHA_IMPLEMENTATION
#define JACK_IMPLEMENTATION
#endif
#include "../external/jack/include/jack.h"

#define ROUTER_INIT_CAP 512
typedef unsigned long usize;

typedef enum { GET = 1, POST = 2 } HttpMethod;
typedef enum { OK = 200, NotFound = 404 } StatusCode;
typedef enum { STR = 1, FILE_ = 2, JSON = 3 } ResponseType;

#define STATIC_FOLDER_PATH "static/"

typedef struct __Request__ {
  const char *m_Path;
  HttpMethod m_Method;
} Request;

typedef union {
  char *m_String;
  char *m_FilePath;
  Json m_Json;
} ResponseData;

typedef struct __Response__ {
  StatusCode m_Status;
  ResponseType m_Type;
  ResponseData payload;
} Response;

typedef Response (*AlphaRouteHandler)(Request);

typedef struct {
  char *m_Path;
  HttpMethod m_Method;
  AlphaRouteHandler m_Handler;
} Route;

typedef struct {
  usize m_Capacity;
  usize m_RoutesCount;
  Route m_Routes[ROUTER_INIT_CAP];
} Router;

typedef struct __Alpha__ {
  int m_Fd;
  usize m_Port;
  usize m_BackLog;
  Router m_Router;
} __Alpha__;

typedef struct __Alpha__ *AlphaApp;

AlphaApp Alpha_New(char *Host, unsigned long Port, unsigned long BackLog);
void Alpha_Run(AlphaApp app);
void Alpha_Get(AlphaApp app, char *Path, AlphaRouteHandler Handler);

#define RESPOND_WITH_STR(str)                                                  \
  Response res = {                                                             \
      .m_Type = STR,                                                           \
      .m_Status = OK,                                                          \
      .payload = {.m_String = str},                                            \
  };                                                                           \
  return res

#define RESPOND_WITH_FILE(file_path)                                           \
  Response res = {                                                             \
      .m_Type = FILE_,                                                         \
      .m_Status = OK,                                                          \
      .payload = {.m_FilePath = file_path},                                    \
  };                                                                           \
  return res

#define STATUS_CODE(code)                                                      \
  ((code) == 200   ? "\033[0;32m200\033[0m"                                    \
   : (code) == 404 ? "\033[0;33m404\033[0m"                                    \
                   : "Unknown Status Code")

#endif
