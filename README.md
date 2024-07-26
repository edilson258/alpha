# Alpha HTTP

Build fast and reliable HTTP server in C

## Why Alpha HTTP?

Alpha HTTP is a powerful and versatile library designed to simplify the creation of HTTP servers in C. Here's why you should consider using Alpha HTTP for your projects:

- **Efficiency**: Optimized for performance under heavy loads.
- **Simplicity**: Easy-to-use API for fast server development.
- **Flexibility**: Supports diverse HTTP methods and content types.
- **Customization**: Tailor server behavior to your application.

## Usage

```C
#include "alpha.h"

#define Host "0.0.0.0"
#define Port 8080

Response home(Request req) {
  RESPOND_WITH_HTML("<h1>Home page</h1>");
}

int main() {
  AlphaApp myapp = Alpha_New(Host, Port);
  Alpha_Get(myapp, "/", home);
  Alpha_Run(myapp);
}
```

## Dependecies

- [Jack](https://github.com/edilson258/jack): To work with JSON data
- [Log4C](https://github.com/edilson258/headers-only/log4c): For logging
