#ifndef DJINNI_UTIL_LOGGER
#define DJINNI_UTIL_LOGGER 1

#include <stdio.h>
#include <stdarg.h>

#define DJINNI_LOG_NONE  99
#define DJINNI_LOG_DEVELOPMENT 6
#define DJINNI_LOG_FATAL 5
#define DJINNI_LOG_ERROR 4
#define DJINNI_LOG_WARN  3
#define DJINNI_LOG_INFO  2
#define DJINNI_LOG_DEBUG 1
#define DJINNI_LOG_ALL   0

struct Djinni_Util_LoggerStruct {
  int log_level;

  void (*log_dev)(const char*,...);
  void (*log_fatal)(const char*,...);
  void (*log_error)(const char*,...);
  void (*log_warn)(const char*,...);
  void (*log_info)(const char*,...);
  void (*log_debug)(const char*,...);
};

extern struct Djinni_Util_LoggerStruct Djinni_Util_Logger;

#endif