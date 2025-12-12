#include <assert.h>

#include "../include/log.h"

int
main() {
  int e;

  e = log_info("this is an info log");
  assert(e == 0);
}
