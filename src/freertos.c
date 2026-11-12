#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/log.h"

// There is no system log, so records go to standard error, which the firmware
// routes to wherever its console is. The stream is locked so that records
// written by different tasks don't interleave.
static inline int
log_vprint(const char *message, va_list args) {
  flockfile(stderr);

  int res = vfprintf(stderr, message, args);

  if (res >= 0) res = fputc('\n', stderr);

  fflush(stderr);

  funlockfile(stderr);

  return res < 0 ? -1 : 0;
}

int
log_vdebug(const char *message, va_list args) {
  return log_vprint(message, args);
}

int
log_vinfo(const char *message, va_list args) {
  return log_vprint(message, args);
}

int
log_vwarn(const char *message, va_list args) {
  return log_vprint(message, args);
}

int
log_verror(const char *message, va_list args) {
  return log_vprint(message, args);
}

int
log_vfatal(const char *message, va_list args) {
  log_vprint(message, args);

  exit(1);
}
