// kaldifst/python/csrc/fstinfo.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_FSTINFO_H_
#define KALDIFST_PYTHON_CSRC_FSTINFO_H_

#include <string>

#include "fst/script/info.h"
#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindFstInfo(py::module &m);  // NOLINT

template <typename A>
void PybindFstInfo(py::module &m,  // NOLINT
                   const std::string &class_help_doc = "") {
  using PyClass = fst::Fst<A>;
  m.def(
      "info",
      [](const PyClass &fst, const std::string &arc_filter = "any",
         const std::string &info_type = "auto", bool pipe = false,
         bool test_properties = false, bool fst_verify = true) {
        auto _fst = fst::script::FstClass(fst);
        fst::script::PrintFstInfo(_fst, test_properties, arc_filter, info_type,
                                  fst_verify, pipe);
      },
      R"(
--arc_filter: type = string, default = "any"
Arc filter: one of: "any", "epsilon", "iepsilon", "oepsilon";
this only affects the counts of (co)accessible states, connected states,
and (strongly) connected components
--fst_verify: type = bool, default = true
Verify FST sanity
--info_type: type = string, default = "auto"
Info format: one of: "auto", "long", "short"
--pipe: type = bool, default = false
Send info to stderr, input to stdout
--test_properties: type = bool, default = true
Compute property values (if unknown to FST)
      )",
      py::arg("fst"), py::arg("arc_filter") = "any",
      py::arg("info_type") = "auto", py::arg("pipe") = false,
      py::arg("test_properties") = false, py::arg("fst_verify") = true);
}

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_FSTINFO_H_
