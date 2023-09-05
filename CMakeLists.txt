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

# Remember to also change ./scripts/conda/kaldifst/meta.yaml
set(KALDIFST_VERSION "1.7.1")

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No CMAKE_BUILD_TYPE given, default to Release")
  set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

option(KALDIFST_BUILD_TESTS "Whether to build tests or not" OFF)
option(KALDIFST_BUILD_PYTHON "Whether to build Python" ON)
option(BUILD_SHARED_LIBS "Whether to build shared libraries" ON)

if(BUILD_SHARED_LIBS AND MSVC)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(NOT BUILD_SHARED_LIBS AND MSVC)
  # see https://cmake.org/cmake/help/latest/prop_tgt/MSVC_RUNTIME_LIBRARY.html
  # https://stackoverflow.com/questions/14172856/compile-with-mt-instead-of-md-using-cmake
  if(MSVC)
      add_compile_options(
          $<$<CONFIG:>:/MT> #---------|
          $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
          $<$<CONFIG:Release>:/MT> #--|
      )
  endif()
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS}")

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


set(CMAKE_CXX_STANDARD 14 CACHE STRING "The C++ version to be used.")

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
