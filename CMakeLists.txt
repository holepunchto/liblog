cmake_minimum_required(VERSION 3.22)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

project(log C)

add_library(log OBJECT)

set_target_properties(
  log
  PROPERTIES
  C_STANDARD 99
  POSITION_INDEPENDENT_CODE 1
)

target_sources(
  log
  INTERFACE
    include/log.h
  PRIVATE
    src/shared.c
)

if(UNIX)
  target_sources(
    log
    PRIVATE
      src/unix.c
  )
endif()

if(WIN32)
  target_sources(
    log
    PRIVATE
      src/win.c
  )
endif()

target_include_directories(
  log
  PUBLIC
    include
)

add_library(log_shared SHARED $<TARGET_OBJECTS:log>)

set_target_properties(
  log_shared
  PROPERTIES
  OUTPUT_NAME log
)

add_library(log_static STATIC $<TARGET_OBJECTS:log>)

set_target_properties(
  log_static
  PROPERTIES
  OUTPUT_NAME log
)
install(TARGETS log_shared log_static)

install(FILES include/log.h DESTINATION include)

if(PROJECT_IS_TOP_LEVEL)
  enable_testing()
  add_subdirectory(test)
endif()
