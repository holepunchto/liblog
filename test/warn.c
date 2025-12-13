#include <assert.h>

#include "../include/log.h"

int
main() {
  int e;

  e = log_warn("this is a warning log");
  assert(e == 0);
}
