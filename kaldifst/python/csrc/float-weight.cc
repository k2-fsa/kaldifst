// kaldifst/python/csrc/float-weight.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/float-weight.h"

#include "fst/float-weight.h"

namespace kaldifst {

static void PybindTropicalWeight(py::module &m) {
  using PyClass = fst::TropicalWeight;
  py::class_<PyClass, fst::FloatWeight>(m, "TropicalWeight")
      .def(py::init<>())
      .def(py::init<float>(), py::arg("f"))
      .def("member", &PyClass::Member)
      .def("quantize", &PyClass::Quantize, py::arg("delta") = fst::kDelta)
      .def("reverse", &PyClass::Reverse)
      .def_static("zero", &PyClass::Zero)
      .def_static("one", &PyClass::One)
      .def_static("no_weight", &PyClass::NoWeight)
      .def_static("type", &PyClass::Type, py::return_value_policy::reference)
      .def_static("properties", &PyClass::Properties);

  m.def("plus",
        [](const PyClass &w1, const PyClass &w2) { return fst::Plus(w1, w2); });

  m.def("times", [](const PyClass &w1, const PyClass &w2) {
    return fst::Times(w1, w2);
  });

  m.def("divide", [](const PyClass &w1, const PyClass &w2) {
    return fst::Divide(w1, w2);
  });
}

void PybindFloatWeight(py::module &m) {
  using PyClass = fst::FloatWeight;
  py::class_<PyClass>(m, "FloatWeight")
      .def(py::init<>())
      .def(py::init<float>(), py::arg("f"))
      .def_property_readonly("value", &PyClass::Value,
                             py::return_value_policy::reference)
      .def("hash", &PyClass::Hash)
      .def("__eq__",
           [](const PyClass &w1, const PyClass &w2) { return w1 == w2; })
      .def("__eq__",
           [](const PyClass &w1, const float &w2) { return w1 == w2; })
      .def("__str__", [](const PyClass &w) {
        std::ostringstream os;
        os << w.Value();
        return os.str();
      });

  PybindTropicalWeight(m);
}

}  // namespace kaldifst
