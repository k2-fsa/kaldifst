// kaldifst/python/csrc/fst.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fst.h"

#include "fst/fst.h"

namespace kaldifst {

static void PybindFstHeader(py::module &m) {
  using PyClass = fst::FstHeader;
  py::class_<PyClass>(m, "FstHeader")
      .def(py::init<>())
      .def_property("fst_type", &PyClass::FstType, &PyClass::SetFstType)
      .def_property("arc_type", &PyClass::ArcType, &PyClass::SetArcType)
      .def_property("version", &PyClass::Version, &PyClass::SetVersion)
      .def_property("flags", &PyClass::GetFlags, &PyClass::SetFlags)
      .def_property("properties", &PyClass::Properties, &PyClass::SetProperties)
      .def_property("start", &PyClass::Start, &PyClass::SetStart)
      .def_property("num_states", &PyClass::NumStates, &PyClass::SetNumStates)
      .def_property("num_arcs", &PyClass::NumArcs, &PyClass::SetNumArcs)
      .def("__str__", &PyClass::DebugString);
}

void PybindFst(py::module &m) {}

}  // namespace kaldifst
