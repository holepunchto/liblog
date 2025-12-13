#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <syslog.h>

#include "../include/log.h"

int
log_vdebug(const char *message, va_list args) {
  vsyslog(LOG_DEBUG, message, args);

  return 0;
}

int
log_vinfo(const char *message, va_list args) {
  vsyslog(LOG_INFO, message, args);

  return 0;
}

int
log_vwarn(const char *message, va_list args) {
  vsyslog(LOG_WARNING, message, args);

  return 0;
}

int
log_verror(const char *message, va_list args) {
  vsyslog(LOG_ERR, message, args);

  return 0;
}

int
log_vfatal(const char *message, va_list args) {
  vsyslog(LOG_EMERG, message, args);

  exit(1);
}
