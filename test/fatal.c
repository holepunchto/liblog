#include <assert.h>
#include <stdbool.h>

#include "../include/log.h"

int
main() {
  int e;

  log_fatal("this is a fatal log");

  assert(false); // Should exit before this
}
