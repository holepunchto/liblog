#import <stdarg.h>
#import <stdlib.h>
#import <string.h>

#include <android/log.h>

#include "../include/log.h"

typedef struct log_s log_t;

struct log_s {
  char *name;
  int flags;
};

static log_t *log_;

int
log_open (const char *name, int flags) {
  if (log_ != NULL) return -1;

  log_ = malloc(sizeof(log_t));
  log_->name = strdup(name);
  log_->flags = flags;

  return 0;
}

int
log_close () {
  if (log_ == NULL) return -1;

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

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  err = __android_log_vprint(ANDROID_LOG_DEBUG, log_->name, "%s", args);

  return err == 1 ? 0 : -1;
}

int
log_vinfo (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  err = __android_log_vprint(ANDROID_LOG_INFO, log_->name, "%s", args);

  return err == 1 ? 0 : -1;
}

int
log_vwarn (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  err = __android_log_vprint(ANDROID_LOG_WARN, log_->name, "%s", args);

  return err == 1 ? 0 : -1;
}

int
log_verror (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  err = __android_log_vprint(ANDROID_LOG_ERROR, log_->name, "%s", args);

  return err == 1 ? 0 : err;
}

int
log_vfatal (const char *message, va_list args) {
  if (log_ == NULL) goto done;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) goto close;

  __android_log_vprint(ANDROID_LOG_FATAL, log_->name, "%s", args);

close:
  log_close();

done:
  exit(1);
}
