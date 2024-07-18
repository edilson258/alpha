#define ALPHA_IMPLEMENTATION
#include "../include/alpha.h"

#define Host "0.0.0.0" /* match any addr */
#define Port 8080
#define BackLog 1024

Response home(Request req) {
  static char *text =
      "<h1>Welcome to the home page</h1>"
      "<h3>This is a humble eCommerce</h3><br/>"
      "Find the list of products <a href=\"/products\">here</a>";

  // constructing a Response object manually
  Response res = {.m_Status = OK, .m_Type = STR, .payload = {.m_String = text}};
  return res;
}

Response products(Request req) {
  char *products =
      "{"
      "\"products\": ["
      "{"
      "\"id\": 1,"
      "\"name\": \"Laptop\","
      "\"price\": 999,"
      "\"description\": \"Powerful laptop for all your computing needs.\","
      "\"category\": \"Computers\""
      "},"
      "{"
      "\"id\": 2,"
      "\"name\": \"Wireless Mouse\","
      "\"price\": 29,"
      "\"description\": \"Ergonomic mouse with wireless connectivity.\","
      "\"category\": \"Peripherals\""
      "},"
      "{"
      "\"id\": 3,"
      "\"name\": \"External SSD Drive\","
      "\"price\": 149,"
      "\"description\": \"High-speed external SSD storage for data backup.\","
      "\"category\": \"Storage\""
      "},"
      "{"
      "\"id\": 4,"
      "\"name\": \"Bluetooth Headphones\","
      "\"price\": 79,"
      "\"description\": \"Wireless headphones with noise-cancelling "
      "features.\","
      "\"category\": \"Audio\""
      "},"
      "{"
      "\"id\": 5,"
      "\"name\": \"Smartphone\","
      "\"price\": 699,"
      "\"description\": \"Feature-rich smartphone with advanced camera "
      "capabilities.\","
      "\"category\": \"Mobile Devices\""
      "}"
      "]}";

  Json json = Json_Parse(products);
  Response res = {.m_Status = 200, .m_Type = JSON, .payload.m_Json = json};
  return res;
}

int main() {
  AlphaApp myapp = Alpha_New(Host, Port, BackLog);
  Alpha_Get(myapp, "/", home);
  Alpha_Get(myapp, "/products", products);
  Alpha_Run(myapp);
}
