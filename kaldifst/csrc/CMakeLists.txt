
include_directories(${CMAKE_SOURCE_DIR})
set(srcs
  kaldi-io.cc
  kaldi-table.cc
  text-utils.cc
  parse-options.cc
  )

add_library(kaldifst_core
  ${srcs}
  )
