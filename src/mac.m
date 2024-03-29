#import <stdarg.h>
#import <stdlib.h>
#import <string.h>

#import <os/log.h>

#import "../include/log.h"

struct log_s {
  char *name;
  int flags;
  os_log_t log;
};

static log_t *log_;

int
log_open (const char *name, int flags) {
  if (log_ != NULL) return -1;

  log_ = malloc(sizeof(log_t));
  log_->name = strdup(name);
  log_->flags = flags;
  log_->log = os_log_create(name, "");

  return 0;
}

int
log_close () {
  if (log_ == NULL) return -1;

  [log_->log release];

  free(log_->name);
  free(log_);

  return 0;
}

int
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

  return 0;
}

int
log_vfatal (const char *message, va_list args) {
  if (log_ == NULL) goto done;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) goto close;

  os_log_fault(log_->log, "%{public}s", formatted);

close:
  log_close();

done:
  exit(1);
}
