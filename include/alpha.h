#ifndef ALPHA_HTTP
#define ALPHA_HTTP

#define ROUTER_INIT_CAP 512

typedef unsigned long _usize;

typedef enum { GET = 1, POST = 2 } HttpMethod;
typedef enum { OK = 1, NotFound = 2 } StatusCode;
typedef enum { STR = 1, FILE_ = 2 } ResponseType;

#define STATIC_FOLDER_PATH "static/"

typedef struct __Request__ {
  const char *m_Path;
  HttpMethod m_Method;
} Request;

typedef union {
  char *m_String;
  char *m_FilePath;
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
  _usize m_Capacity;
  _usize m_RoutesCount;
  Route m_Routes[ROUTER_INIT_CAP];
} Router;

typedef struct __Alpha__ {
  int m_Fd;
  _usize m_Port;
  _usize m_BackLog;
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

#endif
