// kaldifst/python/csrc/text-normalizer.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "kaldifst/csrc/text-normalizer.h"

#include <string>

#include "kaldifst/python/csrc/text-normalizer.h"

namespace kaldifst {

void PybindTextNormalizer(py::module *m) {
  using PyClass = TextNormalizer;
  py::class_<PyClass>(*m, "TextNormalizer")
      .def(py::init<const std::string &>(), py::arg("rule"))
      .def("normalize", &PyClass::Normalize, py::arg("s"),
           py::arg("remove_output_zero") = true)
      .def("__call__", &PyClass::Normalize, py::arg("s"),
           py::arg("remove_output_zero") = true);
}

}  // namespace kaldifst
