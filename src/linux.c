#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "../include/log.h"

typedef struct log_s log_t;

struct log_s {};

static log_t *log_;

int
log_open (const char *name, int flags) {
  if (log_ != NULL) return -1;

  log_ = malloc(sizeof(log_t));

  openlog(name, 0, LOG_USER);

  return 0;
}

int
log_close () {
  if (log_ == NULL) return -1;

  closelog();

  free(log_);

  return 0;
}

int
log_vdebug (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  vsyslog(LOG_DEBUG, message, args);

  return 0;
}

int
log_vinfo (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  vsyslog(LOG_INFO, message, args);

  return 0;
}

int
log_vwarn (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  vsyslog(LOG_WARNING, message, args);

  return 0;
}

int
log_verror (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  vsyslog(LOG_ERR, message, args);

  return 0;
}

int
log_vfatal (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  vsyslog(LOG_EMERG, message, args);

  exit(1);
}
