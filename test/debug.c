#include <assert.h>

#include "../include/log.h"

int
main () {
  int e;

  e = log_open("test", 0);
  assert(e == 0);

  e = log_debug("this is a debug log");
  assert(e == 0);

  e = log_close();
  assert(e == 0);
}
