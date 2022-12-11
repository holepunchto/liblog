list(APPEND tests
  debug
  error
  fatal
  info
  warn
)

foreach(test IN LISTS tests)
  add_executable(${test} ${test}.c)

  target_link_libraries(
    ${test}
    PRIVATE
      log_shared
  )

  target_include_directories(
    ${test}
    PRIVATE
      $<TARGET_PROPERTY:log,INCLUDE_DIRECTORIES>
  )

  add_test(
    NAME ${test}
    COMMAND ${test}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )
endforeach()
