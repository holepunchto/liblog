#include <assert.h>

#include "../include/log.h"

static log_t *log;

int
main () {
  log_open("test", 0, &log);

  log_error("this is an error log");

  log_close(log);
}
