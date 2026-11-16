#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdarg.h>

#if defined(_WIN32)
#if defined(LOG_BUILDING)
#define LOG_EXPORT __declspec(dllexport)
#elif defined(LOG_SHARED)
#define LOG_EXPORT __declspec(dllimport)
#else
#define LOG_EXPORT
#endif
#elif defined(__GNUC__)
#define LOG_EXPORT __attribute__((visibility("default")))
#else
#define LOG_EXPORT
#endif

LOG_EXPORT int
log_vdebug(const char *message, va_list args);

LOG_EXPORT inline int
log_debug(const char *message, ...) {
  va_list args;
  va_start(args, message);

  int err = log_vdebug(message, args);

  va_end(args);

  return err;
}

LOG_EXPORT int
log_vinfo(const char *message, va_list args);

LOG_EXPORT inline int
log_info(const char *message, ...) {
  va_list args;
  va_start(args, message);

  int err = log_vinfo(message, args);

  va_end(args);

  return err;
}

LOG_EXPORT int
log_vwarn(const char *message, va_list args);

LOG_EXPORT inline int
log_warn(const char *message, ...) {
  va_list args;
  va_start(args, message);

  int err = log_vwarn(message, args);

  va_end(args);

  return err;
}

LOG_EXPORT int
log_verror(const char *message, va_list args);

LOG_EXPORT inline int
log_error(const char *message, ...) {
  va_list args;
  va_start(args, message);

  int err = log_verror(message, args);

  va_end(args);

  return err;
}

LOG_EXPORT int
log_vfatal(const char *message, va_list args);

LOG_EXPORT inline int
log_fatal(const char *message, ...) {
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
