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

static void PybindFstWriteOptions(py::module &m) {
  using PyClass = fst::FstWriteOptions;
  py::class_<PyClass>(m, "FstWriteOptions")
      .def_readwrite("source", &PyClass::source, "Where you're writing to.")
      .def_readwrite("write_header", &PyClass::write_header,
                     "Write the header?")
      .def_readwrite("write_isymbols", &PyClass::write_isymbols,
                     "Write input symbols?")
      .def_readwrite("write_osymbols", &PyClass::write_osymbols,
                     "Write output symbols?")
      .def_readwrite("align", &PyClass::align,
                     "Write data aligned (may fail on pipes)?")
      .def_readwrite("stream_write", &PyClass::stream_write,
                     "Avoid seek operations in writing.")
      .def(py::init<const string &, bool, bool, bool, bool, bool>(),
           py::arg("source") = "<unspecified>", py::arg("write_header") = true,
           py::arg("write_isymbols") = true, py::arg("write_osymbols") = true,
           py::arg("align") = FLAGS_fst_align, py::arg("stream_write") = false)
      .def("__str__", [](const PyClass &self) {
        std::ostringstream os;
        os << "source: " << self.source << "\n"
           << "write_header: " << self.write_header << "\n"
           << "write_isymbols: " << self.write_isymbols << "\n"
           << "write_osymbols: " << self.write_osymbols << "\n"
           << "align: " << self.align << "\n"
           << "stream_write: " << self.stream_write << "\n";
        return os.str();
      });
}

static void PybindFstReadOptions(py::module &m) {
  using PyClass = fst::FstReadOptions;
  auto fst_read_options =
      py::class_<fst::FstReadOptions>(m, "FstReadOptions")
          .def(py::init<const fst::string &, const fst::FstHeader *,
                        const fst::SymbolTable *, const fst::SymbolTable *>(),
               py::arg("source") = "<unspecified>", py::arg("header") = nullptr,
               py::arg("isymbols") = nullptr, py::arg("osymbols") = nullptr)
          .def(py::init<const fst::string &, const fst::SymbolTable *,
                        const fst::SymbolTable *>(),
               py::arg("source"), py::arg("isymbols") = nullptr,
               py::arg("osymbols") = nullptr)
          .def_readwrite("source", &fst::FstReadOptions::source,
                         "Where you're reading from.")
          .def_readwrite("header", &fst::FstReadOptions::header,
                         "Pointer to FST header; if non-zero, use this info "
                         "(don't read a stream header).",
                         py::return_value_policy::reference)
          .def_readwrite("isymbols", &fst::FstReadOptions::isymbols,
                         "Pointer to input symbols; if non-zero, use this info "
                         "(read and skip stream isymbols)",
                         py::return_value_policy::reference)
          .def_readwrite("osymbols", &fst::FstReadOptions::osymbols,
                         "Pointer to output symbols; if non-zero, use this "
                         "info (read and skip stream osymbols)",
                         py::return_value_policy::reference)
          .def_readwrite("mode", &fst::FstReadOptions::mode,
                         "Read or map files (advisory, if possible)")
          .def_readwrite("read_isymbols", &fst::FstReadOptions::read_isymbols,
                         "Read isymbols, if any (default: true).")
          .def_readwrite("read_osymbols", &fst::FstReadOptions::read_osymbols,
                         "Read osymbols, if any (default: true).")
          .def_static("read_mode", &fst::FstReadOptions::ReadMode,
                      "Helper function to convert strings FileReadModes into "
                      "their enum value.",
                      py::arg("mode"))
          .def("__str__", &fst::FstReadOptions::DebugString,
               "Outputs a debug string for the FstReadOptions object.");

  py::enum_<fst::FstReadOptions::FileReadMode>(
      fst_read_options, "FileReadMode", py::arithmetic(),
      "FileReadMode(s) are advisory, there are "
      "many conditions than prevent a\n"
      "file from being mapped, READ mode will "
      "be selected in these cases with\n"
      "a warning indicating why it was chosen.")
      .value("READ", fst::FstReadOptions::FileReadMode::READ)
      .value("MAP", fst::FstReadOptions::FileReadMode::MAP)
      .export_values();
}
void PybindMatchType(py::module &m) {
  py::enum_<fst::MatchType>(m, "MatchType", py::arithmetic(),
                            "Specifies matcher action.")
      .value("MATCH_INPUT", fst::MatchType::MATCH_INPUT, "Match input label.")
      .value("MATCH_OUTPUT", fst::MatchType::MATCH_OUTPUT,
             "Match output label.")
      .value("MATCH_BOTH", fst::MatchType::MATCH_BOTH,
             "Match input or output label.")
      .value("MATCH_NONE", fst::MatchType::MATCH_NONE, "Match nothing.")
      .value("MATCH_UNKNOWN", fst::MatchType::MATCH_UNKNOWN,
             "match type unknown.")
      .export_values();
}

void PybindFst(py::module &m) {
  PybindFstHeader(m);
  PybindFstWriteOptions(m);
  PybindFstReadOptions(m);
  PybindMatchType(m);

  PybindStateIteratorBaseImpl<fst::StdArc>(m, "StdArcStateIteratorBase");
  PybindStateIteratorDataImpl<fst::StdArc>(m, "StdArcStateIteratorData");
  PybindArcIteratorBaseImpl<fst::StdArc>(m, "StdArcArcIteratorBase");
  PybindArcIteratorDataImpl<fst::StdArc>(m, "StdArcArcIteratorData");

  PybindFst<fst::StdArc>(
      m, "StdFst",
      "A generic FST, templated on the arc definition, with \n"
      "common-demoninator methods (use StateIterator and \n"
      "ArcIterator to iterate over its states and arcs).");
  PybindStateIteratorImpl<fst::StdFst>(m, "StdFstStateIterator");
  PybindArcIteratorImpl<fst::StdFst>(m, "StdFstArcIterator");

  PybindFstImpl<fst::StdArc>(m, "StdFstImpl");
}

}  // namespace kaldifst
