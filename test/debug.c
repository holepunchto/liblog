#include <assert.h>

#include "../include/log.h"

int
main() {
  int e;

  e = log_debug("this is a debug log");
  assert(e == 0);
}
