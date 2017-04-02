cmake_minimum_required(VERSION 2.8)
project(dataScience)

SET (PROGRAM_NAME "\"${PROJECT_NAME}\"" )
SET (DS_VERSION_MAJOR 0)
SET (DS_VERSION_MINOR 1)
SET (DS_VERSION_PATCH 0)
SET (DS_DEBUG_OPTION $ENV{DS_DEBUG_OPTION})
SET (DS_DEBUG_ENABLED "Debug")
SET (DS_DEBUG_DISABLED "Opt")
SET (DS_VINTAGE_YEAR 2017)
SET (DS_VINTAGE_MONTH 03)
CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/config.h.in" "${PROJECT_BINARY_DIR}/dsConfig.h")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
   message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")


if (DS_DEBUG_OPTION STREQUAL ${DS_DEBUG_ENABLED})
    message(STATUS "Compiling ${DS_DEBUG_OPTION} version")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")
elseif (DS_DEBUG_OPTION STREQUAL ${DS_DEBUG_DISABLED})
    message(STATUS "Compiling ${DS_DEBUG_OPTION} version")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -DDSDEBUG")
else ()
    message(FATAL_ERROR "DS_DEBUG_OPTION environment variable must be set to either ${DS_DEBUG_DISABLED} or ${DS_DEBUG_ENABLED}")
endif ()

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

set (LIB_DIR ${CMAKE_SOURCE_DIR}/lib${DS_DEBUG_OPTION})
set (BIN_DIR ${CMAKE_SOURCE_DIR}/bin${DS_DEBUG_OPTION})
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${LIB_DIR})
set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

message (STATUS "LIB_DIR is ${LIB_DIR}")
message (STATUS "BIN_DIR is ${LIB_DIR}")

add_subdirectory(${CMAKE_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_SOURCE_DIR}/applications)
