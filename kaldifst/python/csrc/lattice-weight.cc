// kaldifst/python/csrc/lattice-weight.cc
//
// Copyright (c)  2021-2023  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/lattice-weight.h"

#include "kaldifst/csrc/lattice-weight.h"

namespace kaldifst {

void PybindLatticeWeight(py::module *m) {
  using PyClass = fst::LatticeWeight;
  py::class_<PyClass>(*m, "LatticeWeight")
      .def(py::init<>())
      .def(py::init<float, float>(), py::arg("graph_cost"),
           py::arg("acoustic_cost"))
      .def_property_readonly_static("zero",
                                    [](py::object) { return PyClass::Zero(); })
      .def_property_readonly_static("one",
                                    [](py::object) { return PyClass::One(); })
      .def_property_readonly_static(
          "no_weight", [](py::object) { return PyClass::NoWeight(); })
      .def_property_readonly_static(
          "type", [](py::object) { return PyClass::Type(); },
          py::return_value_policy::reference)
      .def("member", &PyClass::Member)
      .def("quantize", &PyClass::Quantize, py::arg("delta") = fst::kDelta)
      .def_property_readonly_static(
          "properties", [](py::object) { return PyClass::Properties(); })
      .def("hash", &PyClass::Hash)
      .def("__eq__",
           [](const PyClass &w1, const PyClass &w2) { return w1 == w2; })
      .def("__ne__",
           [](const PyClass &w1, const PyClass &w2) { return w1 != w2; })
      .def("__str__", [](const PyClass &w) {
        std::ostringstream os;
        os << w.Value1() << ", " << w.Value2();
        return os.str();
      });
  ;

  m->def("plus", [](const PyClass &w1, const PyClass &w2) {
    return fst::Plus(w1, w2);
  });

  m->def("times", [](const PyClass &w1, const PyClass &w2) {
    return fst::Times(w1, w2);
  });

  m->def("divide", [](const PyClass &w1, const PyClass &w2) {
    return fst::Divide(w1, w2);
  });

  m->def("approx_equal", [](const PyClass &w1, const PyClass &w2) {
    return fst::ApproxEqual(w1, w2);
  });
}

}  // namespace kaldifst
