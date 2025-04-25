if (CMAKE_VERSION VERSION_GREATER_EQUAL "4.0.0")
  set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
endif()

if("x${CMAKE_SOURCE_DIR}" STREQUAL "x${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "\
In-source build is not a good practice.
Please use:
  mkdir build
  cd build
  cmake ..
to build this project"
  )
endif()

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

project(kaldifst CXX)

set(KALDIFST_VERSION "1.7.13")

# Disable warning about
#
# "The DOWNLOAD_EXTRACT_TIMESTAMP option was not given and policy CMP0135 is
#  not set.
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No CMAKE_BUILD_TYPE given, default to Release")
  set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

option(KALDIFST_BUILD_TESTS "Whether to build tests or not" OFF)
option(KALDIFST_BUILD_PYTHON "Whether to build Python" ON)
option(BUILD_SHARED_LIBS "Whether to build shared libraries" ON)

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS}")

if(BUILD_SHARED_LIBS AND MSVC)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(NOT APPLE)
  set(kaldifst_rpath_origin "$ORIGIN")
else()
  set(kaldifst_rpath_origin "@loader_path")
endif()

set(CMAKE_INSTALL_RPATH ${kaldifst_rpath_origin})
set(CMAKE_BUILD_RPATH ${kaldifst_rpath_origin})


if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14 CACHE STRING "The C++ version to be used.")
endif()

message(STATUS "C++ Standard version: ${CMAKE_CXX_STANDARD}")
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

if(WIN32)
  set(disabled_warnings
    /wd4018
    /wd4244
    /wd4267
    /wd4291
    /wd4305
    /wd4996
  )

  message(STATUS "Disabled warnings: ${disabled_warnings}")
  foreach(w IN LISTS disabled_warnings)
    string(APPEND CMAKE_CXX_FLAGS " ${w} ")
  endforeach()
endif()

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

if(KALDIFST_BUILD_PYTHON)
  include(pybind11)
endif()

include(openfst)
if(KALDIFST_BUILD_TESTS)
  enable_testing()
  include(googletest)
endif()

add_subdirectory(kaldifst)
