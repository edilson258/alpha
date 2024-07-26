#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef enum {
  INFO = 1,
  WARN = 2,
  ERROR = 3,
} LogLevel;

/// Will convert a LogLevel enum entry to its respective string representation
#define LogLevel(level)                                                        \
  (level == INFO)    ? "\033[0;32mINFO\033[0m"                                 \
  : (level == WARN)  ? "\033[0;33mWARNING\033[0m"                              \
  : (level == ERROR) ? "\033[0;31mERROR\033[0m"                                \
                     : "UNKNOWN LOG LEVEL"
/**
 * Function: Log(FILE *file, LogLevel level, Str_t message, ...)
 *
 * will log a message to the specified file
 *
 */
void Log(FILE *file, LogLevel level, char *message, ...);

#ifdef LOG4C_IMPLEMENTATION

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *datetime_str() {
  time_t current_time;
  time(&current_time);
  struct tm *local_time = localtime(&current_time);
  char *time_str = (char *)malloc(sizeof(char) * 20);
  strftime(time_str, 19, "%Y-%m-%d %H:%M:%S", local_time);
  return time_str;
}

void Log(FILE *file, LogLevel level, char *message, ...) {
  fprintf(file, "[%s] %s ", datetime_str(), LogLevel(level));
  va_list args;
  va_start(args, message);
  vfprintf(file, message, args);
  va_end(args);
  fprintf(file, "\n");
}
#endif // LOG4C_IMPLEMENTATION

#endif // LOGGER_H
