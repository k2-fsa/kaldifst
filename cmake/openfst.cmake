# Copyright (c)  2020  Xiaomi Corporation (author: Fangjun Kuang)

function(download_openfst)
  include(FetchContent)

  set(openfst_URL  "https://github.com/csukuangfj/openfst/archive/refs/heads/openfst-1.8.5.zip")
  set(openfst_HASH "SHA256=7a0001d8891b48bd6312cb00818a1d327e7dc3edce164b264a5a6e926358720a")

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
  add_subdirectory(${openfst_SOURCE_DIR} ${openfst_BINARY_DIR} EXCLUDE_FROM_ALL)
  set(openfst_SOURCE_DIR ${openfst_SOURCE_DIR} PARENT_SCOPE)

  set_target_properties(fst PROPERTIES OUTPUT_NAME "kaldifst_fst")

  install(TARGETS fst
    DESTINATION lib
  )

  if(KALDIFST_BUILD_PYTHON)
    set_target_properties(fstscript PROPERTIES OUTPUT_NAME "kaldifst_fstscript")
  endif()

  if(KALDIFST_BUILD_PYTHON AND WIN32)
    install(TARGETS fst fstscript DESTINATION ..)
  else()
    install(TARGETS fst fstscript DESTINATION lib)
  endif()

  if(WIN32 AND BUILD_SHARED_LIBS)
    install(TARGETS fst fstscript DESTINATION bin)
  endif()

endfunction()

download_openfst()
