cmake_minimum_required(VERSION 3.25)

project(log C)

if(APPLE)
  enable_language(OBJC)
endif()

add_library(log OBJECT)

set_target_properties(
  log
  PROPERTIES
  C_STANDARD 99
  POSITION_INDEPENDENT_CODE ON
)

target_sources(
  log
  INTERFACE
    include/log.h
  PRIVATE
    src/shared.c
)

if(APPLE)
  target_sources(
    log
    PRIVATE
      src/mac.m
  )
elseif(UNIX)
  target_sources(
    log
    PRIVATE
      src/linux.c
  )
elseif(WIN32)
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

if(APPLE)
  target_link_libraries(
    log
    PUBLIC
      "-framework Foundation"
  )
endif()

add_library(log_shared SHARED)

set_target_properties(
  log_shared
  PROPERTIES
  OUTPUT_NAME log
  WINDOWS_EXPORT_ALL_SYMBOLS ON
)

target_link_libraries(
  log_shared
  PUBLIC
    log
)

add_library(log_static STATIC)

set_target_properties(
  log_static
  PROPERTIES
  OUTPUT_NAME log
)

target_link_libraries(
  log_static
  PUBLIC
    log
)

install(TARGETS log_shared log_static)

install(FILES include/log.h DESTINATION include)

if(PROJECT_IS_TOP_LEVEL)
  enable_testing()

  add_subdirectory(test)
endif()
