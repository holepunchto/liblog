list(APPEND tests
  debug
  error
  fatal
  info
  warn
)

list(APPEND failing_tests
  fatal
)

foreach(test IN LISTS tests)
  add_executable(${test} ${test}.c)

  target_link_libraries(
    ${test}
    PRIVATE
      log
  )

  add_test(
    NAME ${test}
    COMMAND ${test}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  )

  set_tests_properties(
    ${test}
    PROPERTIES
    TIMEOUT 30
  )

   if(${test} IN_LIST failing_tests)
    set_tests_properties(
      ${test}
      PROPERTIES
      WILL_FAIL TRUE
    )
  endif()
endforeach()
