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
  mutable-fst.cc
  pre-determinize.cc
  symbol-table.cc
  table-matcher.cc
  vector-fst.cc
)
target_link_libraries(_kaldifst PRIVATE kaldifst_core fstscript)

if(APPLE)
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGE_DIR
  )
  message(STATUS "PYTHON_SITE_PACKAGE_DIR: ${PYTHON_SITE_PACKAGE_DIR}")
  target_link_libraries(_kaldifst PRIVATE "-Wl,-rpath,${PYTHON_SITE_PACKAGE_DIR}")
endif()

if(NOT WIN32)
  target_link_libraries(_kaldifst PRIVATE "-Wl,-rpath,${kaldifst_rpath_origin}/kaldifst/lib")
endif()

install(TARGETS _kaldifst
  DESTINATION ../
)
