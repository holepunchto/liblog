cmake_minimum_required(VERSION 4.0)

find_package(cmake-prelude REQUIRED PATHS node_modules/cmake-prelude)
find_package(cmake-test REQUIRED PATHS node_modules/cmake-test)

project(log LANGUAGES C VERSION 0.0.0)

if(WIN32)
  enable_language(CXX)
endif()

# Android ships a system library named liblog.so, which a shared library of
# ours would clash with.
if(ANDROID)
  set(types STATIC)
else()
  set(types STATIC SHARED)
endif()

add_library_targets(
  log
  TYPE
    ${types}
  HEADERS
    include/log.h
  FEATURES
    c_std_99
  SOURCES
    src/shared.c
)

if(ANDROID)
  target_sources(
    log
    PRIVATE
      src/android.c
  )

  target_link_libraries(
    log_headers
    INTERFACE
      -llog
  )
elseif(APPLE)
  target_sources(
    log
    PRIVATE
      src/apple.c
  )
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeRTOS")
  target_sources(
    log
    PRIVATE
      src/freertos.c
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
      src/win32.cc
      src/win32/TraceLoggingDynamic.h
  )

  target_compile_features(
    log
    PRIVATE
      cxx_std_20
  )
endif()

add_test_subdirectory(test)
