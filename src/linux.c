#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>

#include "../include/log.h"

struct log_s {
  char *name;
  int flags;
};

static log_t *log;

int
log_open (const char *name, int flags) {
  if (log != NULL) return -1;

  log = malloc(sizeof(log_t));
  log->name = strdup(name);
  log->flags = flags;

  openlog(name, 0, LOG_USER);

  return 0;
}

int
log_close () {
  if (log == NULL) return -1;

  closelog();

  free(log->name);
  free(log);

  return 0;
}

int
log_vdebug (const char *message, va_list args) {
  if (log == NULL) return -1;

  vsyslog(LOG_DEBUG, message, args);

  return 0;
}

int
log_vinfo (const char *message, va_list args) {
  if (log == NULL) return -1;

  vsyslog(LOG_INFO, message, args);

  return 0;
}

int
log_vwarn (const char *message, va_list args) {
  if (log == NULL) return -1;

  vsyslog(LOG_WARNING, message, args);

  return 0;
}

int
log_verror (const char *message, va_list args) {
  if (log == NULL) return -1;

  vsyslog(LOG_ERR, message, args);

  return 0;
}

int
log_vfatal (const char *message, va_list args) {
  if (log != NULL) {
    vsyslog(LOG_EMERG, message, args);

    log_close();
  }

  exit(1);
}
