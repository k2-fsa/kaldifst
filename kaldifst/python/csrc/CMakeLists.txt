include_directories(${CMAKE_SOURCE_DIR})
include_directories(${openfst_SOURCE_DIR}/src/include)

pybind11_add_module(_kaldifst
  arc.cc
  float-weight.cc
  kaldifst.cc
  symbol-table.cc
)
target_link_libraries(_kaldifst PRIVATE kaldifst_core)
if(UNIX AND NOT APPLE)
  target_link_libraries(_kaldifst PUBLIC ${PYTHON_LIBRARY})
endif()
