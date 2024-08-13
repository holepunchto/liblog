#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <os/log.h>

#include "../include/log.h"

typedef struct log_s log_t;

struct log_s {
  os_log_t log;
};

static log_t *log_;

int
log_open (const char *name, int flags) {
  if (log_ != NULL) return -1;

  log_ = malloc(sizeof(log_t));
  log_->log = os_log_create(name, "");

  return 0;
}

int
log_close () {
  if (log_ == NULL) return -1;

  os_release(log_->log);

  free(log_);

  return 0;
}

static inline int
log_vformat (char **result, size_t *size, const char *message, va_list args) {
  va_list args_copy;
  va_copy(args_copy, args);

  int res = vsnprintf(NULL, 0, message, args);

  va_end(args_copy);

  if (res < 0) return res;

  *size = res + 1 /* NULL */;
  *result = malloc(*size);

  va_copy(args_copy, args);

  vsnprintf(*result, *size, message, args);

  va_end(args_copy);

  return 0;
}

int
log_vdebug (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  if (!os_log_debug_enabled(log_->log)) return 0;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log_debug(log_->log, "%{public}s", formatted);

  free(formatted);

  return 0;
}

int
log_vinfo (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  if (!os_log_info_enabled(log_->log)) return 0;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log_info(log_->log, "%{public}s", formatted);

  free(formatted);

  return 0;
}

int
log_vwarn (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log(log_->log, "%{public}s", formatted);

  free(formatted);

  return 0;
}

int
log_verror (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log_error(log_->log, "%{public}s", formatted);

  free(formatted);

  return 0;
}

int
log_vfatal (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log_fault(log_->log, "%{public}s", formatted);

  free(formatted);

  exit(1);
}
