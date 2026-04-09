# Copyright (c)  2020  Xiaomi Corporation (author: Fangjun Kuang)

function(download_openfst)
  include(FetchContent)

  set(openfst_URL  "https://github.com/csukuangfj/openfst/archive/refs/heads/openfst-1.8.5.zip")
  set(openfst_HASH "SHA256=dbfcbd9f41895a96ca54d70dfddca59e6333831fce0a49d70c84455e5c7e5da0")

  # If you don't have access to the Internet,
  # please pre-download it
  set(possible_file_locations
    $ENV{HOME}/Downloads/openfst-openfst-1.8.5.zip
    ${CMAKE_SOURCE_DIR}/openfst-openfst-1.8.5.zip
    ${CMAKE_BINARY_DIR}/openfst-openfst-1.8.5.zip
    /tmp/openfst-openfst-1.8.5.zip
    /star-fj/fangjun/download/github/openfst-openfst-1.8.5.zip
  )

  foreach(f IN LISTS possible_file_locations)
    if(EXISTS ${f})
      set(openfst_URL  "${f}")
      file(TO_CMAKE_PATH "${openfst_URL}" openfst_URL)
      break()
    endif()
  endforeach()

  set(HAVE_BIN OFF CACHE BOOL "" FORCE)
  set(HAVE_SCRIPT ON CACHE BOOL "" FORCE)
  set(HAVE_COMPACT OFF CACHE BOOL "" FORCE)
  set(HAVE_COMPRESS OFF CACHE BOOL "" FORCE)
  set(HAVE_CONST OFF CACHE BOOL "" FORCE)
  set(HAVE_FAR OFF CACHE BOOL "" FORCE)
  set(HAVE_GRM OFF CACHE BOOL "" FORCE)
  set(HAVE_PDT OFF CACHE BOOL "" FORCE)
  set(HAVE_MPDT OFF CACHE BOOL "" FORCE)
  set(HAVE_LINEAR OFF CACHE BOOL "" FORCE)
  set(HAVE_LOOKAHEAD OFF CACHE BOOL "" FORCE)
  set(HAVE_NGRAM OFF CACHE BOOL "" FORCE)
  set(HAVE_PYTHON OFF CACHE BOOL "" FORCE)
  set(HAVE_SPECIAL OFF CACHE BOOL "" FORCE)

  FetchContent_Declare(openfst
    URL               ${openfst_URL}
    URL_HASH          ${openfst_HASH}
  )

  FetchContent_GetProperties(openfst)
  if(NOT openfst_POPULATED)
    message(STATUS "Downloading openfst from ${openfst_URL}")
    FetchContent_Populate(openfst)
  endif()
  message(STATUS "openfst is downloaded to ${openfst_SOURCE_DIR}")

  set(_build_shared_libs_bak ${BUILD_SHARED_LIBS})
  set(BUILD_SHARED_LIBS OFF)

  add_subdirectory(${openfst_SOURCE_DIR} ${openfst_BINARY_DIR} EXCLUDE_FROM_ALL)

  if(_build_shared_libs_bak)
    set_target_properties(fst fstscript
      PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        C_VISIBILITY_PRESET hidden
        CXX_VISIBILITY_PRESET hidden
    )
    set(BUILD_SHARED_LIBS ON)
  endif()

  set(openfst_SOURCE_DIR ${openfst_SOURCE_DIR} PARENT_SCOPE)

  set_target_properties(fst PROPERTIES OUTPUT_NAME "kaldifst_fst")

  if(KALDIFST_BUILD_PYTHON)
    set_target_properties(fstscript PROPERTIES OUTPUT_NAME "kaldifst_fstscript")
  endif()
endfunction()

download_openfst()
