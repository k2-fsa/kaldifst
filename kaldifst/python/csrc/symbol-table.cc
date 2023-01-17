// kaldifst/python/csrc/symbol-table.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/symbol-table.h"

#include <string>
#include <utility>
#include <vector>

#include "fst/symbol-table.h"

namespace kaldifst {

static void PybindSymbolTableReadOptions(py::module &m) {  // NOLINT
  using PyClass = fst::SymbolTableReadOptions;
  py::class_<PyClass>(m, "SymbolTableReadOptions")
      .def(py::init<>())
      .def(py::init<std::vector<std::pair<int64_t, int64_t>>,
                    const fst::string>(),
           py::arg("string_hash_ranges"), py::arg("source"))
      .def_readwrite("string_hash_ranges", &PyClass::string_hash_ranges)
      .def_readwrite("source", &PyClass::source);
}

static void PybindSymbolTableTextOptions(py::module &m) {  // NOLINT
  using PyClass = fst::SymbolTableTextOptions;
  py::class_<PyClass>(m, "SymbolTableTextOptions")
      .def(py::init<bool>(), py::arg("allow_negative_labels") = false)
      .def_readwrite("allow_negative_labels", &PyClass::allow_negative_labels)
      .def_readwrite("fst_field_separator", &PyClass::fst_field_separator);
}

void PybindSymbolTable(py::module &m) {  // NOLINT
  PybindSymbolTableReadOptions(m);
  PybindSymbolTableTextOptions(m);
  using PyClass = fst::SymbolTable;
  py::class_<PyClass> sym(m, "SymbolTable");
  sym.attr("kNoSymbol") = PyClass::kNoSymbol;
  sym.def(py::init<>())
      .def(py::init<const fst::string &>(), py::arg("name"))
      .def_static(
          "from_str",
          [](const std::string &s) -> PyClass * {
            std::stringstream ss(s);
            return PyClass::ReadText(ss, "from_str");
          },
          py::arg("s"), py::return_value_policy::take_ownership)
      .def_static("read_text",
                  overload_cast_<const fst::string &,
                                 const fst::SymbolTableTextOptions &>()(
                      &PyClass::ReadText),
                  "Reads a text representation of the symbol table",
                  py::arg("filename"),
                  py::arg("opts") = fst::SymbolTableTextOptions(),
                  py::return_value_policy::take_ownership)
      .def_static("read", overload_cast_<const fst::string &>()(&PyClass::Read),
                  "Reads a binary dump of the symbol table.",
                  py::arg("filename"), py::return_value_policy::take_ownership)
      .def("copy", &PyClass::Copy, "Creates a reference counted copy.",
           py::return_value_policy::take_ownership)
      .def("add_symbol",
           (int64_t(PyClass::*)(const fst::string &, int64))(
               &PyClass::AddSymbol),
           "Adds a symbol with given key to table. A symbol table also keeps "
           "track of the last available key (highest key value in the symbol "
           "table).",
           py::arg("symbol"), py::arg("key"))
      .def("add_symbol",
           (int64_t(PyClass::*)(const fst::string &))(&PyClass::AddSymbol),
           "Adds a symbol to the table. The associated value key is "
           "automatically assigned by the symbol table.",
           py::arg("symbol"))
      .def("add_table", &PyClass::AddTable,
           "Adds another symbol table to this table. All key values will be "
           "offset by the current available key (highest key value in the "
           "symbol table). Note string symbols with the same key value will "
           "still have the same key value after the symbol table has been "
           "merged, but a different value. Adding symbol tables do not result "
           "in changes in the base table.",
           py::arg("table"))
      .def("remove_symbol", &PyClass::RemoveSymbol, py::arg("key"))
      .def_property("name", &PyClass::Name, &PyClass::SetName)
      .def("write",
           (bool (PyClass::*)(const fst::string &) const)(&PyClass::Write),
           py::arg("filename"))
      .def("write_text",
           (bool (PyClass::*)(const fst::string &) const)(&PyClass::WriteText),
           "Dump a text representation of the symbol table.",
           py::arg("filename"))
      .def("find", (fst::string(PyClass::*)(int64) const)(&PyClass::Find),
           "Returns the string associated with the key; if the key is out of"
           "range (<0, >max), returns an empty string.",
           py::arg("key"))
      .def("find",
           (int64(PyClass::*)(const fst::string &) const)(&PyClass::Find),
           "Returns the key associated with the symbol; if the symbol does "
           "not exist, kNoSymbol is returned.",
           py::arg("symbol"))
      .def("find", (int64(PyClass::*)(const char *) const)(&PyClass::Find),
           "Returns the key associated with the symbol; if the symbol does "
           "not exist, kNoSymbol is returned.",
           py::arg("symbol"))
      .def("__contains__",
           [](const PyClass &self, py::object o) -> bool {
             if (py::isinstance<py::int_>(o)) {
               return self.Member(py::cast<int>(o));
             } else {
               return self.Member(py::str(o));
             }
           })

      .def("available_key", &PyClass::AvailableKey,
           "Returns the current available key (i.e., highest key + 1) in the "
           "symbol table.")
      .def("num_symbols", &PyClass::NumSymbols,
           "Returns the current number of symbols in table (not necessarily "
           "equal to available_key()).")
      .def("__str__",
           [](const PyClass &sym) {
             std::ostringstream os;
             sym.WriteText(os);
             return os.str();
           })
      .def_property_readonly(
          "check_sum", &PyClass::CheckSum,
          "Return the label-agnostic MD5 check-sum for this table. All new "
          "symbols added to the table will result in an updated checksum. "
          "Deprecated.")
      .def_property_readonly(
          "labeled_check_sum", &PyClass::LabeledCheckSum,
          "Same as `check_sum`, but returns an label-dependent version.");
}

}  // namespace kaldifst
