#include <assert.h>

#include "../include/log.h"

int
main() {
  int e;

  e = log_error("this is an error log");
  assert(e == 0);
}
