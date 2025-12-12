#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <os/log.h>

#include "../include/log.h"

static inline int
log_vformat(char **result, size_t *size, const char *message, va_list args) {
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
log_vdebug(const char *message, va_list args) {
  if (!os_log_debug_enabled(OS_LOG_DEFAULT)) return 0;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log_debug(OS_LOG_DEFAULT, "%{public}s", formatted);

  free(formatted);

  return 0;
}

int
log_vinfo(const char *message, va_list args) {
  if (!os_log_info_enabled(OS_LOG_DEFAULT)) return 0;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log_info(OS_LOG_DEFAULT, "%{public}s", formatted);

  free(formatted);

  return 0;
}

int
log_vwarn(const char *message, va_list args) {
  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log(OS_LOG_DEFAULT, "%{public}s", formatted);

  free(formatted);

  return 0;
}

int
log_verror(const char *message, va_list args) {
  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log_error(OS_LOG_DEFAULT, "%{public}s", formatted);

  free(formatted);

  return 0;
}

int
log_vfatal(const char *message, va_list args) {
  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  os_log_fault(OS_LOG_DEFAULT, "%{public}s", formatted);

  free(formatted);

  exit(1);
}
