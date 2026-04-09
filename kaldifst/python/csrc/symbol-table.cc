// kaldifst/python/csrc/symbol-table.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/symbol-table.h"

#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "fst/symbol-table.h"

namespace kaldifst {

void PybindSymbolTable(py::module &m) {  // NOLINT
  using PyClass = fst::SymbolTable;
  py::class_<PyClass> sym(m, "SymbolTable");
  sym.attr("kNoSymbol") = fst::kNoSymbol;
  sym.def(py::init<>())
      .def(py::init<const std::string &>(), py::arg("name"))
      .def_static(
          "from_str",
          [](const std::string &s) -> PyClass * {
            std::stringstream ss(s);
            return PyClass::ReadText(ss, "from_str");
          },
          py::arg("s"), py::return_value_policy::take_ownership)
      .def_static(
          "read_text",
          [](const std::string &filename) -> PyClass * {
            return PyClass::ReadText(filename);
          },
          "Reads a text representation of the symbol table",
          py::arg("filename"), py::return_value_policy::take_ownership)
      .def_static(
          "read",
          static_cast<PyClass *(*)(const std::string &)>(&PyClass::Read),
          "Reads a binary dump of the symbol table.",
          py::arg("filename"), py::return_value_policy::take_ownership)
      .def("copy", &PyClass::Copy, "Creates a reference counted copy.",
           py::return_value_policy::take_ownership)
      .def("add_symbol",
           (int64_t (PyClass::*)(std::string_view, int64_t))(
                &PyClass::AddSymbol),
           "Adds a symbol with given key to table. A symbol table also keeps "
           "track of the last available key (highest key value in the symbol "
           "table).",
           py::arg("symbol"), py::arg("key"))
      .def("add_symbol",
           (int64_t (PyClass::*)(std::string_view))(&PyClass::AddSymbol),
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
           (bool (PyClass::*)(const std::string &) const)(&PyClass::Write),
           py::arg("filename"))
       .def("write_text",
           [](const PyClass &self, const std::string &filename) -> bool {
             return self.WriteTextWithStatus(filename).ok();
           },
           "Dump a text representation of the symbol table.",
            py::arg("filename"))
       .def("find", (std::string (PyClass::*)(int64_t) const)(&PyClass::Find),
           "Returns the string associated with the key; if the key is out of"
           "range (<0, >max), returns an empty string.",
           py::arg("key"))
       .def("find",
           (int64_t (PyClass::*)(std::string_view) const)(&PyClass::Find),
           "Returns the key associated with the symbol; if the symbol does "
           "not exist, kNoSymbol is returned.",
           py::arg("symbol"))
      .def("__contains__",
           [](const PyClass &self, py::object o) -> bool {
              if (py::isinstance<py::int_>(o)) {
                return self.Member(py::cast<int>(o));
              } else {
                return self.Member(py::cast<std::string>(o));
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
              if (!sym.WriteTextWithStatus(os).ok()) {
                throw std::runtime_error("Failed to stringify SymbolTable");
              }
              return os.str();
            })
       .def_property_readonly(
          "check_sum",
          [](const PyClass &self) { return self.LabeledCheckSum(); },
          "Deprecated alias for labeled_check_sum.")
       .def_property_readonly(
           "labeled_check_sum", &PyClass::LabeledCheckSum,
           "Same as `check_sum`, but returns an label-dependent version.");
}

}  // namespace kaldifst
