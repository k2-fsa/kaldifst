// kaldifst/python/csrc/fst.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_FST_H_
#define KALDIFST_PYTHON_CSRC_FST_H_

#include <memory>
#include <string>

#include "fst/fst.h"
#include "fst/script/print.h"
#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindFst(py::module &m);  // NOLINT

template <typename A>
void PybindStateIteratorBaseImpl(py::module &m,  // NOLINT
                                 const std::string &class_name,
                                 const std::string &class_help_doc = "") {
  using PyClass = fst::StateIteratorBase<A>;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def_property_readonly("done", &PyClass::Done, "End of iterator?")
      .def_property_readonly("value", &PyClass::Value,
                             "Returns current state (when !Done()).")
      .def("next", &PyClass::Next, "Advances to next state (when !Done()).")
      .def("reset", &PyClass::Reset, "Resets to initial condition.");
}

template <typename A>
void PybindStateIteratorDataImpl(py::module &m,  // NOLINT
                                 const std::string &class_name,
                                 const std::string &class_help_doc = "") {
  using PyClass = fst::StateIteratorData<A>;
  py::class_<PyClass, std::unique_ptr<PyClass, py::nodelete>>(
      m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def_readwrite("base", &PyClass::base,
                     "Specialized iterator if non-zero.")
      .def_readwrite("nstates", &PyClass::nstates,
                     "Otherwise, the total number of states.");
}

template <typename A>
void PybindArcIteratorBaseImpl(py::module &m,  // NOLINT
                               const std::string &class_name,
                               const std::string &class_help_doc = "") {
  using PyClass = fst::ArcIteratorBase<A>;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def_property_readonly("done", &PyClass::Done, "End of iterator?")
      .def_property_readonly("value", &PyClass::Value,
                             "Returns current arc (when !Done()).",
                             py::return_value_policy::reference)
      .def("next", &PyClass::Next, "Advances to next arc (when !Done()).")
      .def_property_readonly("position", &PyClass::Position,
                             "Returns current position.")
      .def("reset", &PyClass::Reset, "Resets to initial condition.")
      .def("seek", &PyClass::Seek, "Advances to arbitrary arc by position.")
      .def_property_readonly("flags", &PyClass::Flags,
                             "Returns current behavorial flags.")
      .def("set_flags", &PyClass::SetFlags, "Sets behavorial flags.");
}

template <typename A>
void PybindArcIteratorDataImpl(py::module &m,  // NOLINT
                               const std::string &class_name,
                               const std::string &class_help_doc = "") {
  using PyClass = fst::ArcIteratorData<A>;
  py::class_<PyClass, std::unique_ptr<PyClass, py::nodelete>>(
      m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def_readwrite("base", &PyClass::base,
                     "Specialized iterator if non-zero.")
      .def_readwrite("arcs", &PyClass::arcs, "Otherwise arcs pointer")
      .def_readwrite("narcs", &PyClass::narcs, "arc count")
      .def_readwrite("ref_count", &PyClass::ref_count,
                     "reference count if non-zero.");
}

template <typename FST>
void PybindStateIteratorImpl(py::module &m,  // NOLINT
                             const std::string &class_name,
                             const std::string &class_help_doc = "") {
  using PyClass = fst::StateIterator<FST>;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<const FST &>(), py::arg("fst"))
      .def_property_readonly("done", &PyClass::Done)
      .def_property_readonly("value", &PyClass::Value)
      .def("next", &PyClass::Next)
      .def("reset", &PyClass::Reset);
}

template <typename FST>
void PybindArcIteratorImpl(py::module &m,  // NOLINT
                           const std::string &class_name,
                           const std::string &class_help_doc = "") {
  using PyClass = fst::ArcIterator<FST>;
  using StateId = typename FST::StateId;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<const FST &, StateId>(), py::arg("fst"), py::arg("s"))
      .def_property_readonly("done", &PyClass::Done)
      .def_property_readonly("value", &PyClass::Value,
                             py::return_value_policy::reference)
      .def("next", &PyClass::Next)
      .def("reset", &PyClass::Reset)
      .def("seek", &PyClass::Seek, py::arg("a"))
      .def_property_readonly("position", &PyClass::Position)
      .def_property_readonly("flags", &PyClass::Flags)
      .def("set_flags", &PyClass::SetFlags);
}

template <typename A>
void PybindFst(py::module &m,  // NOLINT
               const std::string &class_name,
               const std::string &class_help_doc = "") {
  using PyClass = fst::Fst<A>;
  using Arc = typename PyClass::Arc;
  using StateId = typename PyClass::StateId;
  using Weight = typename PyClass::Weight;

  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def_property_readonly("start", &PyClass::Start)
      .def("final", &PyClass::Final, "State's final weight.")
      .def("num_arcs", &PyClass::NumArcs, "State's arc count.")
      .def("num_input_epsilons", &PyClass::NumInputEpsilons,
           "State's output epsilon count.")
      .def("properties", &PyClass::Properties,
           "Property bits. If test = false, return stored properties bits "
           "for mask\n"
           "(some possibly unknown); if test = true, return property bits "
           "for mask\n"
           "(computing o.w. unknown).",
           py::arg("mask"), py::arg("test"))
      .def_property_readonly("type", &PyClass::Type)
      .def_property_readonly("is_olabel_sorted",
                             [](const PyClass &self) -> bool {
                               return self.Properties(fst::kOLabelSorted,
                                                      true) != 0;
                             })
      .def_property_readonly("is_ilabel_sorted",
                             [](const PyClass &self) -> bool {
                               return self.Properties(fst::kILabelSorted,
                                                      true) != 0;
                             })
      .def("copy", &PyClass::Copy,
           "Gets a copy of this Fst. The copying behaves as follows:\n",
           "\n"
           "(1) The copying is constant time if safe = false or if safe = "
           "true and is on an otherwise unaccessed FST.\n"
           "\n"
           "(2) If safe = true, the copy is thread-safe in that the original\n"
           "and copy can be safely accessed (but not necessarily mutated) by\n"
           "separate threads. For some FST types, 'Copy(true)' should only\n"
           "be called on an FST that has not otherwise been accessed.\n"
           "Behavior is otherwise undefined.\n"
           "\n"
           "(3) If a MutableFst is copied and then mutated, then the original"
           "\n"
           "is unmodified and vice versa (often by a copy-on-write on the \n"
           "initial mutation, which may not be constant time).\n"
           "\n"
           "  .. warning::\n\n"
           "      To get a deep copy of an FST, e.g., a deep copy of Lattice "
           "``lat1``, please use:\n\n"
           "      .. code-block:: python\n\n"
           "          lat2 = kaldifst.Lattice(lat1)\n\n\n"
           "\n"
           "      To get deep copy of StdVectorFst ``fst1``, please use\n\n"
           "      .. code-block:: python\n\n"
           "          fst2 = kaldifst.StdVectorFst(fst1)\n\n",
           py::arg("safe") = false, py::return_value_policy::take_ownership)
      .def_static(
          "read", overload_cast_<const fst::string &>()(&PyClass::Read),
          "Reads an FST from a file; returns nullptr on error. An empty\n"
          "filename results in reading from standard input.",
          py::arg("filename"), py::return_value_policy::take_ownership)
      .def("write",
           (bool (PyClass::*)(const fst::string &) const)(&PyClass::Write),
           "Writes an FST to a file; returns false on error; an empty\n"
           "filename results in writing to standard output.",
           py::arg("filename"))
      .def_property_readonly(
          "input_symbols", &PyClass::InputSymbols,
          "Returns input label symbol table; return nullptr if not "
          "specified.",
          py::return_value_policy::reference)
      .def_property_readonly(
          "output_symbols", &PyClass::OutputSymbols,
          "Returns output label symbol table; return nullptr if not "
          "specified.",
          py::return_value_policy::reference)
      .def("init_state_iterator", &PyClass::InitStateIterator,
           "For generic state iterator construction (not normally called "
           "directly by users). Does not copy the FST.",
           py::arg("data"))
      .def("init_arc_iterator", &PyClass::InitArcIterator,
           "For generic arc iterator construction (not normally called "
           "directly by users). Does not copy the FST.",
           py::arg("s"), py::arg("data"))
      .def("__str__",
           [](const PyClass &self) -> std::string {
             std::ostringstream os;
             auto _fst = fst::script::FstClass(self);
             fst::FstPrinter<Arc>(*_fst.GetFst<Arc>(), _fst.InputSymbols(),
                                  _fst.OutputSymbols(),
                                  nullptr,  // state symbol table, ssyms
                                  false,  // false means not in acceptor format
                                  false,  // false means not to show weight one
                                  "      ",  // fst field separator, 6 spaces
                                  ""         // missing symbol
                                  )
                 .Print(&os, "standard output");
             return os.str();
           })
      .def(
          "to_str",
          [](const PyClass &self, bool is_acceptor = false,
             bool show_weight_one = false,
             const std::string &fst_field_separator = "      ",
             const std::string &missing_symbol = "",
             const std::string &dest = "stardard output") {
            std::ostringstream os;
            auto _fst = fst::script::FstClass(self);
            fst::FstPrinter<Arc>(*_fst.GetFst<Arc>(), _fst.InputSymbols(),
                                 _fst.OutputSymbols(), nullptr, is_acceptor,
                                 show_weight_one, fst_field_separator,
                                 missing_symbol)
                .Print(&os, dest);
            return os.str();
          },
          "see fstprint for help, e.g., fstprint --help",
          py::arg("is_acceptor") = false, py::arg("show_weight_one") = false,
          py::arg("fst_field_separator") = "      ",
          py::arg("missing_symbol") = "", py::arg("dest") = "stardard output");
}

template <typename A>
void PybindFstImpl(py::module &m,  // NOLINT
                   const std::string &class_name,
                   const std::string &class_help_doc = "") {
  using PyClass = fst::internal::FstImpl<A>;

  using StateId = typename PyClass::StateId;
  using Weight = typename PyClass::Weight;

  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def_property("type", &PyClass::Type, &PyClass::SetType)
      .def("properties", (uint64(PyClass::*)() const)(&PyClass::Properties))
      .def("properties",
           (uint64(PyClass::*)(uint64) const)(&PyClass::Properties))
      .def("set_properties",
           (void (PyClass::*)(uint64))(&PyClass::SetProperties),
           py::arg("props"))
      .def("set_properties",
           (void (PyClass::*)(uint64, uint64))(&PyClass::SetProperties),
           py::arg("props"), py::arg("mask"))
      .def("set_properties",
           (void (PyClass::*)(uint64, uint64) const)(&PyClass::SetProperties),
           py::arg("props"), py::arg("mask"))
      .def_property(
          "input_symbols", [](PyClass &self) { return self.InputSymbols(); },
          [](PyClass &self, fst::SymbolTable *isyms) {
            self.SetInputSymbols(isyms);
          })
      .def_property(
          "output_symbols", [](PyClass &self) { return self.OutputSymbols(); },
          [](PyClass &self, fst::SymbolTable *osyms) {
            self.SetOutputSymbols(osyms);
          });
}

template <typename Impl, typename FST = fst::Fst<typename Impl::Arc>>
void PybindImplToFst(py::module &m,  // NOLINT
                     const std::string &class_name,
                     const std::string &class_help_doc = "") {
  using PyClass = fst::ImplToFst<Impl, FST>;
  using Parent = FST;
  using Arc = typename PyClass::Arc;
  using StateId = typename PyClass::StateId;
  using Weight = typename PyClass::Weight;

  py::class_<PyClass, Parent>(m, class_name.c_str(), class_help_doc.c_str());
}

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_FST_H_
