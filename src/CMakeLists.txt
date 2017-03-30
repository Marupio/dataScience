# DS library

#file(GLOB_RECURSE DS_LIBRARY_SOURCES "src/*.cpp")
#file(GLOB_RECURSE DS_LIBRARY_HEADERS "src/*.h")
#
#set (DS_LIBRARY_INCLUDE_DIRS "")
#foreach (_headerFile ${DS_LIBRARY_INCLUDE_DIRS})
#    get_filename_component(_dir ${_headerFile} PATH)
#    list (APPEND DS_LIBRARY_INCLUDE_DIRS ${_dir})
#endforeach()
#list(REMOVE_DUPLICATES DS_LIBRARY_INCLUDE_DIRS)
#
#include_directories(${DS_LIBRARY_INCLUDE_DIRS})

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/core
)

add_library(
    core SHARED
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Card.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/HandRanker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/core/Board.cpp
)
