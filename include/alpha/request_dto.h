#ifndef REQUEST_DTO
#define REQUEST_DTO

#include <linux/in.h>

#include "../alpha.h"

typedef struct {
  int file_descriptor;
  struct sockaddr_in address;
} Client;

typedef struct {
  Client client;
  AlphaApp *app;
} RequestDTO;

#endif
