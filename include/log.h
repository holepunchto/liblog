#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

typedef struct log_s log_t;

int
log_open (const char *name, int flags);

int
log_close ();

int
log_vdebug (const char *message, va_list args);

inline int
log_debug (const char *message, ...) {
  va_list args;
  va_start(args, message);

  int err = log_vdebug(message, args);

  va_end(args);

  return err;
}

int
log_vinfo (const char *message, va_list args);

inline int
log_info (const char *message, ...) {
  va_list args;
  va_start(args, message);

  int err = log_vinfo(message, args);

  va_end(args);

  return err;
}

int
log_vwarn (const char *message, va_list args);

inline int
log_warn (const char *message, ...) {
  va_list args;
  va_start(args, message);

  int err = log_vwarn(message, args);

  va_end(args);

  return err;
}

int
log_verror (const char *message, va_list args);

inline int
log_error (const char *message, ...) {
  va_list args;
  va_start(args, message);

  int err = log_verror(message, args);

  va_end(args);

  return err;
}

int
log_vfatal (const char *message, va_list args);

inline int
log_fatal (const char *message, ...) {
  va_list args;
  va_start(args, message);

  int err = log_vfatal(message, args);

  va_end(args);

  return err;
}

#ifdef __cplusplus
}
#endif

#endif // LOG_H
