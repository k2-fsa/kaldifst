// kaldifst/python/csrc/fstarcsort.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "fst/minimize.h"
#include "fst/shortest-distance.h"
#include "kaldifst/csrc/log.h"
#include "kaldifst/python/csrc/fstarcsort.h"

namespace kaldifst {

void PybindFstMinimize(py::module &m) {
  m.def(
      "minimize",
      [](fst::MutableFst<fst::StdArc> *in_out, double delta = 1e-6,
         bool allow_nondet = false) {
        fst::Minimize<fst::StdArc>(in_out, nullptr, delta, allow_nondet);
      },
      py::arg("in_out"), py::arg("delta") = 1e-6,
      py::arg("allow_nondet") = false);
}

}  // namespace kaldifst
