#include <vector>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/log.h"

#include "win32/TraceLoggingDynamic.h"

using namespace tld;

typedef struct log_s log_t;

struct log_s {
  Provider provider;

  log_s(const char *name) : provider(name) {}
};

namespace {

static log_t *log_;

}

extern "C" int
log_open (const char *name, int flags) {
  if (log_ != NULL) return -1;

  log_ = new log_t(name);

  return 0;
}

extern "C" int
log_close () {
  if (log_ == NULL) return -1;

  delete log_;

  return 0;
}

namespace {

static inline int
log_vformat (char **result, size_t *size, const char *message, va_list args) {
  va_list args_copy;
  va_copy(args_copy, args);

  int res = vsnprintf(NULL, 0, message, args_copy);

  va_end(args_copy);

  if (res < 0) return res;

  *size = res + 1 /* NULL */;
  *result = new char[*size]();

  va_copy(args_copy, args);

  vsnprintf(*result, *size, message, args);

  va_end(args_copy);

  return 0;
}

} // namespace

extern "C" int
log_vdebug (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  Event<std::vector<BYTE>> event("Debug", 5);

  event.AddField("message", Type::TypeUtf8String);
  event.AddString(formatted);

  event.Write(log_->provider);

  delete[] formatted;

  return 0;
}

extern "C" int
log_vinfo (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  Event<std::vector<BYTE>> event("Information", 4);

  event.AddField("message", Type::TypeUtf8String);
  event.AddString(formatted);

  event.Write(log_->provider);

  delete[] formatted;

  return 0;
}

extern "C" int
log_vwarn (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  Event<std::vector<BYTE>> event("Warning", 3);

  event.AddField("message", Type::TypeUtf8String);
  event.AddString(formatted);

  event.Write(log_->provider);

  delete[] formatted;

  return 0;
}

extern "C" int
log_verror (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  Event<std::vector<BYTE>> event("Error", 2);

  event.AddField("message", Type::TypeUtf8String);
  event.AddString(formatted);

  event.Write(log_->provider);

  delete[] formatted;

  return 0;
}

extern "C" int
log_vfatal (const char *message, va_list args) {
  if (log_ == NULL) return -1;

  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  Event<std::vector<BYTE>> event("Critical", 1);

  event.AddField("message", Type::TypeUtf8String);
  event.AddString(formatted);

  event.Write(log_->provider);

  delete[] formatted;

  exit(1);
}
