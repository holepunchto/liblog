#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600 // Windows Vista
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include <TraceLoggingProvider.h>

#include "../include/log.h"

TRACELOGGING_DEFINE_PROVIDER(provider, "liblog", (0xe51b91fb, 0xbf54, 0x53ca, 0xbc, 0x02, 0xd0, 0x22, 0x07, 0x26, 0x97, 0x72));

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

  TraceLoggingRegister(provider);

  return 0;
}

int
log_close () {
  if (log_ == NULL) return -1;

  TraceLoggingUnregister(provider);

  free(log_->name);
  free(log_);

  return 0;
}

int
log_vformat (char **result, size_t *size, const char *message, va_list args) {
  va_list args_copy;
  va_copy(args_copy, args);

  int res = vsnprintf(NULL, 0, message, args_copy);

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

  TraceLoggingWrite(
    provider,
    "Debug",
    TraceLoggingLevel(5),
    TraceLoggingString(log_->name, "log"),
    TraceLoggingString(formatted, "message")
  );

  free(formatted);

  return 0;
}

int
log_vinfo (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  TraceLoggingWrite(
    provider,
    "Information",
    TraceLoggingLevel(4),
    TraceLoggingString(log_->name, "log"),
    TraceLoggingString(formatted, "message")
  );

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

  TraceLoggingWrite(
    provider,
    "Warning",
    TraceLoggingLevel(3),
    TraceLoggingString(log_->name, "log"),
    TraceLoggingString(formatted, "message")
  );

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

  TraceLoggingWrite(
    provider,
    "Error",
    TraceLoggingLevel(2),
    TraceLoggingString(log_->name, "log"),
    TraceLoggingString(formatted, "message")
  );

  free(formatted);

  return 0;
}

int
log_vfatal (const char *message, va_list args) {
  if (log_ == NULL) goto done;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) goto close;

  TraceLoggingWrite(
    provider,
    "Critical",
    TraceLoggingLevel(1),
    TraceLoggingString(log_->name, "log"),
    TraceLoggingString(formatted, "message")
  );

  free(formatted);

close:
  log_close();

done:
  exit(1);
}
