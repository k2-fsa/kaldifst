
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${openfst_SOURCE_DIR}/src/include)

set(srcs
  context-fst.cc
  kaldi-fst-io.cc
  kaldi-holder.cc
  kaldi-io.cc
  kaldi-math.cc
  kaldi-semaphore.cc
  kaldi-table.cc
  parse-options.cc
  text-normalizer.cc
  text-utils.cc
  )

add_library(kaldifst_core ${srcs})
target_link_libraries(kaldifst_core PUBLIC fst)

if(APPLE)
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGE_DIR
  )
  message(STATUS "PYTHON_SITE_PACKAGE_DIR: ${PYTHON_SITE_PACKAGE_DIR}")
  target_link_libraries(kaldifst_core PUBLIC "-L ${PYTHON_SITE_PACKAGE_DIR}/../..")
endif()


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

if(KALDIFST_BUILD_PYTHON AND WIN32)
  install(TARGETS kaldifst_core DESTINATION ..)
else()
  install(TARGETS kaldifst_core DESTINATION lib)
endif()

if(WIN32 AND BUILD_SHARED_LIBS)
  install(TARGETS kaldifst_core DESTINATION bin)
endif()
