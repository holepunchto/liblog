add_test_executables(
  debug.c
  error.c
  info.c
  warn.c
  LINK
    log::<TYPE>
)

add_test_executable(
  fatal
  fatal.c
  LINK
    log::<TYPE>
  PROPERTIES
    WILL_FAIL TRUE
)
