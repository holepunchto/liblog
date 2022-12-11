#include <assert.h>

#include "../include/log.h"

static log_t *log;

int
main () {
  log_open("test", 0, &log);

  log_warn("this is a warning log");

  log_close(log);
}
