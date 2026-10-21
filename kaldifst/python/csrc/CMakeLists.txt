include_directories(${CMAKE_SOURCE_DIR})
include_directories(${openfst_SOURCE_DIR}/src/include)

pybind11_add_module(_kaldifst
  arc.cc
  const-fst.cc
  context-fst.cc
  determinize-star.cc
  expanded-fst.cc
  float-weight.cc
  fst.cc
  fstarcsort.cc
  fstcompile.cc
  fstconnect.cc
  fstdeterminize.cc
  fstdraw.cc
  fstext-utils.cc
  fstinfo.cc
  fstinvert.cc
  fstminimize.cc
  fstreverse.cc
  fstrmepsilon.cc
  kaldi-table.cc
  kaldifst.cc
  lattice-utils.cc
  lattice-weight.cc
  mutable-fst.cc
  pre-determinize.cc
  shortest-path.cc
  symbol-table.cc
  table-matcher.cc
  text-normalizer.cc
  vector-fst.cc
)
target_link_libraries(_kaldifst PRIVATE kaldifst_core fstscript)

if(MSVC)
  target_compile_options(
    _kaldifst
    PRIVATE
      $<$<CONFIG:Release>:/GL->
      $<$<CONFIG:Release>:/Od /Ob0>
      $<$<CONFIG:RelWithDebInfo>:/GL->
      $<$<CONFIG:RelWithDebInfo>:/Od /Ob0>
      $<$<CONFIG:MinSizeRel>:/GL->
      $<$<CONFIG:MinSizeRel>:/Od /Ob0>
  )
  target_link_options(
    _kaldifst
    PRIVATE
      $<$<CONFIG:Release>:/LTCG:OFF>
      $<$<CONFIG:RelWithDebInfo>:/LTCG:OFF>
      $<$<CONFIG:MinSizeRel>:/LTCG:OFF>
  )
endif()

if(APPLE)
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGE_DIR
  )
  message(STATUS "PYTHON_SITE_PACKAGE_DIR: ${PYTHON_SITE_PACKAGE_DIR}")
  if(PYTHON_SITE_PACKAGE_DIR STREQUAL "")
    message(WARNING "PYTHON_SITE_PACKAGE_DIR is empty!")
  else()
    target_link_libraries(_kaldifst PRIVATE "-Wl,-rpath,${PYTHON_SITE_PACKAGE_DIR}/kaldifst/lib")
  endif()
endif()

if(NOT WIN32)
  target_link_libraries(_kaldifst PRIVATE "-Wl,-rpath,${kaldifst_rpath_origin}/kaldifst/lib")
endif()

install(TARGETS _kaldifst DESTINATION lib)

# `kaldifst/__init__.py` does `from kaldifst.lib._kaldifst import ...`, so the
# extension has to sit next to the Python package it belongs to. The tests put
# kaldifst/python on PYTHONPATH and expect to find it under kaldifst/lib/;
# `make install` puts it there for a real install, this covers the build tree.
add_custom_command(TARGET _kaldifst POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
    "${CMAKE_SOURCE_DIR}/kaldifst/python/kaldifst/lib"
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:_kaldifst>"
    "${CMAKE_SOURCE_DIR}/kaldifst/python/kaldifst/lib/"
  COMMENT "Copying $<TARGET_FILE_NAME:_kaldifst> to kaldifst/python/kaldifst/lib"
)
