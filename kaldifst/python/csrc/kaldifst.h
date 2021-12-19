// kaldifst/python/csrc/kaldifst.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_KALDIFST_H_
#define KALDIFST_PYTHON_CSRC_KALDIFST_H_

#include "pybind11/pybind11.h"

namespace py = pybind11;

template <typename... Args>
using overload_cast_ = py::detail::overload_cast_impl<Args...>;

#endif  // KALDIFST_PYTHON_CSRC_KALDIFST_H_
