// kaldifst/python/csrc/vector-fst.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_VECTOR_FST_H_
#define KALDIFST_PYTHON_CSRC_VECTOR_FST_H_

#include <string>
#include <vector>

#include "fst/fst.h"
#include "fst/vector-fst.h"
#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindVectorFst(py::module &m);  // NOLINT

template <typename A>
void PybindVectorState(py::module &m,  // NOLINT
                       const std::string &class_name,
                       const std::string &class_help_doc = "") {
  using PyClass = fst::VectorState<A>;

  using StateId = typename PyClass::StateId;
  using Weight = typename PyClass::Weight;

  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str());
}

template <typename S>
void PybindVectorFstBaseImpl(py::module &m,  // NOLINT
                             const std::string &class_name,
                             const std::string &class_help_doc = "") {
  using PyClass = fst::internal::VectorFstBaseImpl<S>;
  using Parent = fst::internal::FstImpl<typename S::Arc>;
  using StateId = typename PyClass::StateId;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def_property("start", &PyClass::Start, &PyClass::SetStart)
      .def("final", &PyClass::Final, py::arg("state"))
      .def_property_readonly("num_states", &PyClass::NumStates)
      .def("num_arcs", &PyClass::NumArcs, py::arg("state"))
      .def("num_input_epsilons", &PyClass::NumInputEpsilons, py::arg("state"))
      .def("num_output_epsilons", &PyClass::NumOutputEpsilons, py::arg("state"))
      .def("set_final", &PyClass::SetFinal, py::arg("state"), py::arg("weight"))
      .def("add_state", (StateId(PyClass::*)())(&PyClass::AddState))
      .def("add_arc", &PyClass::AddArc, py::arg("state"), py::arg("arc"))
      .def("delete_states",
           (void (PyClass::*)(const std::vector<StateId> &))(
               &PyClass::DeleteStates),
           py::arg("states"))
      .def("delete_states", (void (PyClass::*)())(&PyClass::DeleteStates))
      .def("delete_arcs",
           (void (PyClass::*)(StateId, size_t))(&PyClass::DeleteArcs),
           py::arg("state"), py::arg("n"))
      .def("delete_arcs", (void (PyClass::*)(StateId))(&PyClass::DeleteArcs),
           py::arg("state"))
      .def("reserve_states", &PyClass::ReserveStates, py::arg("n"))
      .def("reserve_arcs", &PyClass::ReserveArcs, py::arg("state"),
           py::arg("n"));
}

template <typename S>
void PybindVectorFstImpl(py::module &m,  // NOLINT
                         const std::string &class_name,
                         const std::string &class_help_doc = "") {
  using PyClass = fst::internal::VectorFstImpl<S>;
  using Parent = fst::internal::VectorFstBaseImpl<S>;
  using Arc = typename PyClass::Arc;
  py::class_<PyClass, Parent>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def(py::init<const fst::Fst<Arc> &>(), py::arg("fst"));
}

template <typename A, typename S = fst::VectorState<A>>
void PybindVectorFst(py::module &m,  // NOLINT
                     const std::string &class_name,
                     const std::string &class_help_doc = "") {
  using PyClass = fst::VectorFst<A>;
  using Parent = fst::ImplToMutableFst<fst::internal::VectorFstImpl<S>>;
  using Arc = typename PyClass::Arc;
  py::class_<PyClass, Parent>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def(py::init<const fst::Fst<Arc> &>(), py::arg("fst"))
      .def_static(
          "read", overload_cast_<const fst::string &>()(&PyClass::Read),
          "Reads an FST from a file; returns nullptr on error. An empty\n"
          "filename results in reading from standard input.",
          py::arg("filename"), py::return_value_policy::take_ownership);
}

template <typename Arc, typename State>
void PybindStateIteratorVectorFst(py::module &m,  // NOLINT
                                  const std::string &class_name,
                                  const std::string &class_help_doc = "") {
  using PyClass = fst::StateIterator<fst::VectorFst<Arc, State>>;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<const fst::VectorFst<Arc, State> &>(), py::arg("fst"))
      .def_property_readonly("done", &PyClass::Done)
      .def_property_readonly("value", &PyClass::Value)
      .def("next", &PyClass::Next)
      .def("reset", &PyClass::Reset);
}

template <typename Arc, typename State>
void PybindArcIteratorVectorFst(py::module &m,  // NOLINT
                                const std::string &class_name,
                                const std::string &class_help_doc = "") {
  using PyClass = fst::ArcIterator<fst::VectorFst<Arc, State>>;
  using StateId = typename PyClass::StateId;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<const fst::VectorFst<Arc, State> &, StateId>(),
           py::arg("fst"), py::arg("state"))
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

#endif  // KALDIFST_PYTHON_CSRC_VECTOR_FST_H_
