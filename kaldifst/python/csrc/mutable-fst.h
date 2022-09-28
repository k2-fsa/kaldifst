// kaldifst/python/csrc/mutable-fst.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_MUTABLE_FST_H_
#define KALDIFST_PYTHON_CSRC_MUTABLE_FST_H_

#include <string>
#include <vector>

#include "fst/mutable-fst.h"
#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindMutableFst(py::module &m);  // NOLINT

template <typename A>
void PybindMutableArcIteratorBase(py::module &m,  // NOLINT
                                  const std::string &class_name,
                                  const std::string &class_help_doc = "") {
  using PyClass = fst::MutableArcIteratorBase<A>;
  using Parent = fst::ArcIteratorBase<A>;
  py::class_<PyClass, Parent>(m, class_name.c_str(), class_help_doc.c_str())
      .def_property("value", &PyClass::Value, &PyClass::SetValue);
}

template <typename FST>
void PybindMutableArcIterator(py::module &m,  // NOLINT
                              const std::string &class_name,
                              const std::string &class_help_doc = "") {
  using PyClass = fst::MutableArcIterator<FST>;
  using StateId = typename PyClass::StateId;

  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<FST *, StateId>(), py::arg("fst"), py::arg("s"))
      .def_property_readonly("done", &PyClass::Done)
      .def_property("Value", &PyClass::Value, &PyClass::SetValue)
      .def("next", &PyClass::Next)
      .def_property_readonly("position", &PyClass::Position)
      .def("reset", &PyClass::Reset)
      .def("seek", &PyClass::Seek, py::arg("a"))
      .def_property("Flags", &PyClass::Flags, &PyClass::SetFlags);
}

template <typename A>
void PybindMutableFst(py::module &m,  // NOLINT
                      const std::string &class_name,
                      const std::string &class_help_doc = "") {
  using PyClass = fst::MutableFst<A>;
  using Parent = fst::ExpandedFst<A>;
  using StateId = typename PyClass::StateId;
  using Weight = typename PyClass::Weight;
  py::class_<PyClass, Parent>(m, class_name.c_str(), class_help_doc.c_str())
      .def_property("start", &PyClass::Start, &PyClass::SetStart)
      .def("set_final", &PyClass::SetFinal, py::arg("state"), py::arg("weight"))
      .def(
          "set_final",
          [](PyClass &self, StateId state, typename Weight::ValueType weight) {
            self.SetFinal(state, weight);
          },
          py::arg("state"), py::arg("weight"))
      .def("set_properties", &PyClass::SetProperties, py::arg("props"),
           py::arg("mask"))
      .def("add_state", &PyClass::AddState)
      .def("add_arc", &PyClass::AddArc, py::arg("state"), py::arg("arc"))
      .def("delete_states",
           (void (PyClass::*)(const std::vector<StateId> &))(
               &PyClass::DeleteStates),
           py::arg("states"))
      .def("delete_states", (void (PyClass::*)())(&PyClass::DeleteStates))
      .def("delete_arcs",
           (void (PyClass::*)(StateId, size_t))(&PyClass::DeleteArcs),
           py::arg("state"), py::arg("n"))
      .def("reserve_states", &PyClass::ReserveStates, py::arg("n"))
      .def("reserve_arcs", &PyClass::ReserveArcs, py::arg("state"),
           py::arg("n"))
      .def_property(
          "input_symbols", [](PyClass &self) { return self.InputSymbols(); },
          [](PyClass &self, fst::SymbolTable *isyms) {
            self.SetInputSymbols(isyms);
          })
      .def_property(
          "output_symbols", [](PyClass &self) { return self.OutputSymbols(); },
          [](PyClass &self, fst::SymbolTable *osyms) {
            self.SetOutputSymbols(osyms);
          })
      .def("init_mutable_arc_iterator", &PyClass::InitMutableArcIterator,
           py::arg("state"), py::arg("data"));
}

template <class Impl, class FST = fst::MutableFst<typename Impl::Arc>>
void PybindImplToMutableFst(py::module &m,  // NOLINT
                            const std::string &class_name,
                            const std::string &class_help_doc = "") {
  using PyClass = fst::ImplToMutableFst<Impl, FST>;
  using Parent = fst::ImplToExpandedFst<Impl, FST>;
  py::class_<PyClass, Parent>(m, class_name.c_str(), class_help_doc.c_str());
}

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_MUTABLE_FST_H_
