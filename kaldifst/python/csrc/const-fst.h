// kaldifst/python/csrc/const-fst.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_CONST_FST_H_
#define KALDIFST_PYTHON_CSRC_CONST_FST_H_

#include "fst/const-fst.h"
#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindConstFst(py::module &m);

template <class A, class U = uint32_t>
void PybindConstFstImpl(py::module &m, const std::string &class_name,
                        const std::string &class_help_doc = "") {
  using PyClass = fst::internal::ConstFstImpl<A, U>;
  using Parent = fst::internal::FstImpl<A>;
  py::class_<PyClass, Parent>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def_property_readonly("start", &PyClass::Start)
      .def("final", &PyClass::Final, py::arg("state"))
      .def_property_readonly("num_states", &PyClass::NumStates)
      .def("num_arcs", &PyClass::NumArcs, py::arg("state"))
      .def("num_input_epsilons", &PyClass::NumInputEpsilons, py::arg("state"))
      .def("num_output_epsilons", &PyClass::NumOutputEpsilons,
           py::arg("state"));
}

template <class A, class U = uint32_t>
void PybindConstFst(py::module &m, const std::string &class_name,
                    const std::string &class_help_doc = "") {
  using PyClass = fst::ConstFst<A, U>;
  using Parent = fst::ImplToExpandedFst<fst::internal::ConstFstImpl<A, U>>;

  using Arc = A;
  py::class_<PyClass, Parent>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def(py::init<const fst::Fst<Arc> &>(), py::arg("fst"));
}

template <typename Arc, typename U = uint32_t>
void PybindStateIteratorConstFst(py::module &m, const std::string &class_name,
                                 const std::string &class_help_doc = "") {
  using PyClass = fst::StateIterator<fst::ConstFst<Arc, U>>;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<const fst::ConstFst<Arc, U> &>(), py::arg("fst"))
      .def_property_readonly("done", &PyClass::Done)
      .def_property_readonly("value", &PyClass::Value)
      .def("next", &PyClass::Next)
      .def("reset", &PyClass::Reset);
}

template <typename Arc, typename U = uint32_t>
void PybindArcIteratorConstFst(py::module &m, const std::string &class_name,
                               const std::string &class_help_doc = "") {
  using PyClass = fst::ArcIterator<fst::ConstFst<Arc, U>>;
  using StateId = typename PyClass::StateId;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<const fst::ConstFst<Arc, U> &, StateId>(), py::arg("fst"),
           py::arg("state"))
      .def_property_readonly("done", &PyClass::Done)
      .def_property_readonly("value", &PyClass::Value,
                             py::return_value_policy::reference)
      .def("next", &PyClass::Next)
      .def("reset", &PyClass::Reset)
      .def("seek", &PyClass::Seek, py::arg("a"))
      .def_property_readonly("position", &PyClass::Position)
      .def_property_readonly("flags", &PyClass::Flags);
}

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_CONST_FST_H_
