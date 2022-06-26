cmake_minimum_required(VERSION 3.11)

project(GTlabLogging)

if(NOT DEFINED CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX "-d")
endif()

set(GTLAB_TARGET_DIR $ENV{GTLAB_DEVTOOLS_DIR}/bin CACHE PATH "Target Dir")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# Compiler settings
if (NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

if (NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS ON)
endif()

add_subdirectory(src)

option(BUILD_UNITTESTS "Build Unittests" OFF)
if (BUILD_UNITTESTS)
    add_subdirectory(tests/unittests)
endif ()
