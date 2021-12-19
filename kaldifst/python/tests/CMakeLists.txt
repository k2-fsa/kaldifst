function(kaldifst_add_py_test source)
  get_filename_component(name ${source} NAME_WE)
  set(name "${name}_py")

  add_test(NAME ${name}
    COMMAND
      "${PYTHON_EXECUTABLE}"
      "${CMAKE_CURRENT_SOURCE_DIR}/${source}"
  )

  get_filename_component(kaldifst_path ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)

  set_property(TEST ${name}
    PROPERTY ENVIRONMENT "PYTHONPATH=${kaldifst_path}:$<TARGET_FILE_DIR:_kaldifst>:$ENV{PYTHONPATH}"
  )
endfunction()

# please sort the files in alphabetic order
set(py_test_files
  test_arc.py
  test_float_weight.py
  test_symbol_table.py
)

foreach(source IN LISTS py_test_files)
  kaldifst_add_py_test(${source})
endforeach()

