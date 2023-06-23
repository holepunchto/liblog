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

static log_t *log;

int
log_open (const char *name, int flags) {
  if (log != NULL) return -1;

  log = malloc(sizeof(log_t));
  log->name = strdup(name);
  log->flags = flags;
  log->log = os_log_create(name, "");

  return 0;
}

int
log_close () {
  if (log == NULL) return -1;

  [log->log release];

  free(log->name);
  free(log);

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
  if (log == NULL) return -1;

  if (!os_log_debug_enabled(log->log)) return 0;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log_debug(log->log, "%{public}s", formatted);

  return 0;
}

int
log_vinfo (const char *message, va_list args) {
  if (log == NULL) return -1;

  if (!os_log_info_enabled(log->log)) return 0;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log_info(log->log, "%{public}s", formatted);

  return 0;
}

int
log_vwarn (const char *message, va_list args) {
  if (log == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log(log->log, "%{public}s", formatted);

  return 0;
}

int
log_verror (const char *message, va_list args) {
  if (log == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log_error(log->log, "%{public}s", formatted);

  return 0;
}

int
log_vfatal (const char *message, va_list args) {
  if (log == NULL) goto done;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) goto close;

  os_log_fault(log->log, "%{public}s", formatted);

close:
  log_close();

done:
  exit(1);
}
