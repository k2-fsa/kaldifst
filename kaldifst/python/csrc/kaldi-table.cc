// kaldi_native_io/python/csrc/kaldi-table.cc
//
// Copyright (c)  2022  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/csrc/kaldi-table.h"

#include <string>

#include "kaldifst/csrc/kaldi-fst-io.h"
#include "kaldifst/csrc/kaldi-holder.h"
#include "kaldifst/python/csrc/kaldi-table.h"

namespace kaldifst {

template <class Holder>
void PybindTableWriter(py::module &m,  // NOLINT
                       const std::string &class_name,
                       const std::string &class_help_doc = "") {
  using PyClass = TableWriter<Holder>;

  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def(py::init<const std::string &>(), py::arg("wspecifier"))
      .def("open", &PyClass::Open, py::arg("wspecifier"))
      .def_property_readonly("is_open", &PyClass::IsOpen)
      .def("write", &PyClass::Write, py::arg("key"), py::arg("value"))
      .def("flush", &PyClass::Flush)
      .def("close", &PyClass::Close);
}

template <class Holder>
void PybindSequentialTableReader(py::module &m,  // NOLINT
                                 const std::string &class_name,
                                 const std::string &class_help_doc = "") {
  using PyClass = SequentialTableReader<Holder>;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def(py::init<const std::string &>(), py::arg("rspecifier"))
      .def("open", &PyClass::Open, py::arg("rspecifier"))
      .def_property_readonly("done", &PyClass::Done)
      .def_property_readonly("key", &PyClass::Key)
      .def("free_current", &PyClass::FreeCurrent)
      .def_property_readonly("value", &PyClass::Value)
      .def("next", &PyClass::Next)
      .def_property_readonly("is_open", &PyClass::IsOpen)
      .def("close", &PyClass::Close);
}

template <class Holder>
void PybindRandomAccessTableReader(py::module &m,  // NOLINT
                                   const std::string &class_name,
                                   const std::string &class_help_doc = "") {
  using PyClass = RandomAccessTableReader<Holder>;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<>())
      .def(py::init<const std::string &>(), py::arg("rspecifier"))
      .def("open", &PyClass::Open, py::arg("rspecifier"))
      .def_property_readonly("is_open", &PyClass::IsOpen)
      .def("close", &PyClass::Close)
      .def("__contains__", &PyClass::HasKey)
      .def("__getitem__", &PyClass::Value, py::arg("key"),
           py::return_value_policy::reference);
}

void PybindKaldiTable(py::module &m) {  // NOLINT
  PybindTableWriter<fst::VectorFstHolder>(m, "_VectorFstWriter");
  PybindSequentialTableReader<fst::VectorFstHolder>(
      m, "_SequentialVectorFstReader");
  PybindRandomAccessTableReader<fst::VectorFstHolder>(
      m, "_RandomAccessVectorFstReader");
}

}  // namespace kaldifst
