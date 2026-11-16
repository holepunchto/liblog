if(TARGET log_shared)
  set(library log::shared)
else()
  set(library log::static)
endif()

add_test_executables(
  debug.c
  error.c
  info.c
  warn.c
  LINK
    ${library}
)

add_test_executable(
  fatal
  fatal.c
  LINK
    ${library}
  PROPERTIES
    WILL_FAIL TRUE
)
