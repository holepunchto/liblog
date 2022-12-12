#include <assert.h>
#include <stdbool.h>

#include "../include/log.h"

int
main () {
  int e;

  e = log_open("test", 0);
  assert(e == 0);

  log_fatal("this is a fatal log");

  assert(false); // Should exit before this
}
