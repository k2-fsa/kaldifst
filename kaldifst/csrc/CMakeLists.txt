
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${openfst_SOURCE_DIR}/src/include)

set(srcs
  kaldi-fst-io.cc
  kaldi-holder.cc
  kaldi-io.cc
  kaldi-semaphore.cc
  kaldi-table.cc
  parse-options.cc
  text-utils.cc
  )

add_library(kaldifst_core ${srcs})
target_link_libraries(kaldifst_core PUBLIC fst)

function(kaldifst_add_test source)
  get_filename_component(name ${source} NAME_WE)
  add_executable(${name} "${source}")
  target_link_libraries(${name}
    PRIVATE
      kaldifst_core
      gtest
      gtest_main
  )

  add_test(NAME "Test.${name}"
    COMMAND
    $<TARGET_FILE:${name}>
  )
endfunction()

if(KALDIFST_BUILD_TESTS)
  # please sort the source files alphabetically
  set(test_srcs
    kaldi-fst-io-test.cc
  )

  foreach(source IN LISTS test_srcs)
    kaldifst_add_test(${source})
  endforeach()
endif()
