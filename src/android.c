#import <stdarg.h>
#import <stdlib.h>
#import <string.h>

#include <android/log.h>

#include "../include/log.h"

int
log_vdebug(const char *message, va_list args) {
  int err = __android_log_vprint(ANDROID_LOG_DEBUG, NULL, message, args);

  return err == 1 ? 0 : -1;
}

int
log_vinfo(const char *message, va_list args) {
  int err = __android_log_vprint(ANDROID_LOG_INFO, NULL, message, args);

  return err == 1 ? 0 : -1;
}

int
log_vwarn(const char *message, va_list args) {
  int err = __android_log_vprint(ANDROID_LOG_WARN, NULL, message, args);

  return err == 1 ? 0 : -1;
}

int
log_verror(const char *message, va_list args) {
  int err = __android_log_vprint(ANDROID_LOG_ERROR, NULL, message, args);

  return err == 1 ? 0 : err;
}

int
log_vfatal(const char *message, va_list args) {
  __android_log_vprint(ANDROID_LOG_FATAL, NULL, message, args);

  exit(1);
}
