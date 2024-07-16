cmake_minimum_required(VERSION 3.29.5)
project(
  ALPHA_HTTP
  VERSION 0.1.0
  DESCRIPTION "A C HTTP library"
  LANGUAGES C)

# Build Alpha Http as a static library
file(GLOB Alpha_Sources "src/*.c")
add_library(alphahttp STATIC ${Alpha_Sources})

add_subdirectory(examples)
