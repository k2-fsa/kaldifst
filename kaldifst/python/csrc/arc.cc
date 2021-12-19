// kaldifst/python/csrc/arc.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/arc.h"

#include "fst/arc.h"

namespace kaldifst {

template <typename W>
static void PybindArcImpl(py::module &m, const std::string &class_name,
                          const std::string &class_help_doc = "") {
  using PyClass = fst::ArcTpl<W>;
  using Weight = typename PyClass::Weight;
  using Label = typename PyClass::Label;
  using StateId = typename PyClass::StateId;

  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def(py::init<Label, Label, Weight, StateId>(), py::arg("ilabel"),
           py::arg("olabel"), py::arg("weight"), py::arg("nextstate"))
      .def(py::init(
               [](Label ilabel, Label olabel, float weight, StateId nextstate) {
                 return std::make_unique<PyClass>(ilabel, olabel, weight,
                                                  nextstate);
               }),
           py::arg("ilabel"), py::arg("olabel"), py::arg("weight"),
           py::arg("nextstate"))
      .def_readwrite("ilabel", &PyClass::ilabel)
      .def_readwrite("olabel", &PyClass::olabel)
      .def_property(
          "weight", [](const PyClass &self) { return self.weight; },
          [](PyClass &self, py::object o) {
            if (py::isinstance<py::float_>(o)) {
              self.weight = py::cast<float>(o);
            } else {
              self.weight = py::cast<Weight>(o);
            }
          })
      .def_readwrite("nextstate", &PyClass::nextstate)
      .def("__str__",
           [](const PyClass &arc) {
             std::ostringstream os;
             os << "(ilabel: " << arc.ilabel << ", "
                << "olabel: " << arc.olabel << ", "
                << "weight: " << arc.weight << ", "
                << "nextstate: " << arc.nextstate << ")";
             return os.str();
           })
      .def_property_readonly_static(
          "type", [](py::object) { return PyClass::Type(); },
          py::return_value_policy::reference);
}

void PybindArc(py::module &m) {
  PybindArcImpl<fst::TropicalWeight>(m, "StdArc");
}

}  // namespace kaldifst
