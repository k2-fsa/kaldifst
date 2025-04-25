// kaldifst/python/csrc/text-normalizer.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "kaldifst/csrc/text-normalizer.h"

#include <string>

#include "kaldifst/python/csrc/text-normalizer.h"

namespace kaldifst {

void PybindTextNormalizer(py::module *m) {
  using PyClass = TextNormalizer;
  using P = std::string (PyClass::*)(const std::string &, bool) const;
  using P2 = std::string (PyClass::*)(const std::vector<std::string> &,
                                      const std::vector<std::string> &) const;

  py::class_<PyClass>(*m, "TextNormalizer")
      .def(py::init<const std::string &>(), py::arg("rule"))
      .def("normalize", (P)(&PyClass::Normalize), py::arg("s"),
           py::arg("remove_output_zero") = true)
      .def("__call__", (P)&PyClass::Normalize, py::arg("s"),
           py::arg("remove_output_zero") = true)
      .def("normalize", (P2)(&PyClass::Normalize), py::arg("words"),
           py::arg("pronunciations"))
      .def("__call__", (P2)&PyClass::Normalize, py::arg("words"),
           py::arg("pronunciations"));
}

}  // namespace kaldifst
