#include <vector>

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/log.h"

#include "win32/TraceLoggingDynamic.h"

using namespace tld;

namespace {

// ETW registration is process wide and `EventWrite()` is thread safe, so one
// provider serves every thread. It is created on first use and never destroyed:
// a dynamic initializer would run on each thread as it starts, including the
// loader threads Windows creates while the process exits, and destroying it at
// exit would leave later logs writing through a dead registration.
static const Provider &
log_provider() {
  static const Provider *provider = [] {
    wchar_t exe[MAX_PATH];

    GetModuleFileNameW(nullptr, exe, MAX_PATH);

    auto name = exe;

    for (auto p = exe; *p; ++p) {
      if (*p == L'\\' || *p == L'/') name = p + 1;
    }

    return new Provider(name);
  }();

  return *provider;
}

static inline int
log_vformat(char **result, size_t *size, const char *message, va_list args) {
  va_list args_copy;
  va_copy(args_copy, args);

  int res = vsnprintf(NULL, 0, message, args_copy);

  va_end(args_copy);

  if (res < 0) return res;

  *size = res + 1 /* NULL */;
  *result = new char[*size]();

  va_copy(args_copy, args);

  vsnprintf(*result, *size, message, args_copy);

  va_end(args_copy);

  return 0;
}

} // namespace

extern "C" int
log_vdebug(const char *message, va_list args) {
  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  Event<std::vector<BYTE>> event("Debug", 5);

  event.AddField("message", Type::TypeUtf8String);
  event.AddString(formatted);

  event.Write(log_provider());

  delete[] formatted;

  return 0;
}

extern "C" int
log_vinfo(const char *message, va_list args) {
  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  Event<std::vector<BYTE>> event("Information", 4);

  event.AddField("message", Type::TypeUtf8String);
  event.AddString(formatted);

  event.Write(log_provider());

  delete[] formatted;

  return 0;
}

extern "C" int
log_vwarn(const char *message, va_list args) {
  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  Event<std::vector<BYTE>> event("Warning", 3);

  event.AddField("message", Type::TypeUtf8String);
  event.AddString(formatted);

  event.Write(log_provider());

  delete[] formatted;

  return 0;
}

extern "C" int
log_verror(const char *message, va_list args) {
  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  Event<std::vector<BYTE>> event("Error", 2);

  event.AddField("message", Type::TypeUtf8String);
  event.AddString(formatted);

  event.Write(log_provider());

  delete[] formatted;

  return 0;
}

extern "C" int
log_vfatal(const char *message, va_list args) {
  char *formatted;
  size_t size;

  int err = log_vformat(&formatted, &size, message, args);
  if (err < 0) return err;

  Event<std::vector<BYTE>> event("Critical", 1);

  event.AddField("message", Type::TypeUtf8String);
  event.AddString(formatted);

  event.Write(log_provider());

  delete[] formatted;

  exit(1);
}
