#include <stdio.h>

#include "../include/alpha.h"

#define Host "0.0.0.0" /* match any addr */
#define Port 8080
#define BackLog 1024

Response home(Request req) {
  printf("Oh! New request for home page\n");

  static char *text =
      "<h1>Welcome to the home page</h1>"
      "<h3>This is a humble eCommerce</h3><br/>"
      "Find the list of products <a href=\"/products\">here</a>";

  // constructing a Response object manually
  Response res = {.m_Status = OK, .m_Type = STR, .payload = {.m_String = text}};
  return res;
}

Response products(Request req) {
  // "index.html" must be under static folder
  RESPOND_WITH_FILE("products.html");
}

int main() {
  AlphaApp myapp = Alpha_New(Host, Port, BackLog);
  Alpha_Get(myapp, "/", home);
  Alpha_Get(myapp, "/products", products);
  Alpha_Run(myapp);
}
