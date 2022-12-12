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

static log_t *log;

int
log_open (const char *name, int flags) {
  if (log != NULL) return -1;

  log = malloc(sizeof(log_t));
  log->name = strdup(name);
  log->flags = flags;

  TraceLoggingRegister(provider);

  return 0;
}

int
log_close () {
  if (log == NULL) return -1;

  TraceLoggingUnregister(provider);

  free(log->name);
  free(log);

  return 0;
}

int
log_vformat (char **result, size_t *size, const char *message, va_list args) {
  int res = vsnprintf(NULL, 0, message, args);
  if (res < 0) return res;

  *size = res + 1 /* NULL */;
  *result = malloc(*size);

  vsnprintf(*result, *size, message, args);

  return 0;
}

int
log_vdebug (const char *message, va_list args) {
  if (log == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  TraceLoggingWrite(
    provider,
    "Debug",
    TraceLoggingLevel(5),
    TraceLoggingString(log->name, "log"),
    TraceLoggingString(formatted, "message")
  );

  free(formatted);

  return 0;
}

int
log_vinfo (const char *message, va_list args) {
  if (log == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  TraceLoggingWrite(
    provider,
    "Information",
    TraceLoggingLevel(4),
    TraceLoggingString(log->name, "log"),
    TraceLoggingString(formatted, "message")
  );

  free(formatted);

  return 0;
}

int
log_vwarn (const char *message, va_list args) {
  if (log == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  TraceLoggingWrite(
    provider,
    "Warning",
    TraceLoggingLevel(3),
    TraceLoggingString(log->name, "log"),
    TraceLoggingString(formatted, "message")
  );

  free(formatted);

  return 0;
}

int
log_verror (const char *message, va_list args) {
  if (log == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  TraceLoggingWrite(
    provider,
    "Error",
    TraceLoggingLevel(2),
    TraceLoggingString(log->name, "log"),
    TraceLoggingString(formatted, "message")
  );

  free(formatted);

  return 0;
}

int
log_vfatal (const char *message, va_list args) {
  log_verror(message, args); // Discard error

  if (log != NULL) log_close(log);

  exit(1);
}
