#import <stdarg.h>
#import <stdlib.h>
#import <string.h>

#include <android/log.h>

#include "../include/log.h"

typedef struct log_s log_t;

struct log_s {
  char *name;
};

static log_t *log_;

int
log_open (const char *name, int flags) {
  if (log_ != NULL) return -1;

  log_ = malloc(sizeof(log_t));
  log_->name = strdup(name);

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
log_vdebug (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  int err = __android_log_vprint(ANDROID_LOG_DEBUG, log_->name, message, args);

  return err == 1 ? 0 : -1;
}

int
log_vinfo (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  int err = __android_log_vprint(ANDROID_LOG_INFO, log_->name, message, args);

  return err == 1 ? 0 : -1;
}

int
log_vwarn (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  int err = __android_log_vprint(ANDROID_LOG_WARN, log_->name, message, args);

  return err == 1 ? 0 : -1;
}

int
log_verror (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  int err = __android_log_vprint(ANDROID_LOG_ERROR, log_->name, message, args);

  return err == 1 ? 0 : err;
}

int
log_vfatal (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  __android_log_vprint(ANDROID_LOG_FATAL, log_->name, message, args);

  exit(1);
}
