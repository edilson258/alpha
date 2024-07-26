cmake_minimum_required(VERSION 3.29.5)
project(
  ALPHA_HTTP
  VERSION 0.1.0
  DESCRIPTION "A C HTTP library"
  LANGUAGES C)

file(GLOB Alpha_Sources "src/*.c")
add_library(alpha STATIC ${Alpha_Sources})
target_link_libraries(alpha cthings)
