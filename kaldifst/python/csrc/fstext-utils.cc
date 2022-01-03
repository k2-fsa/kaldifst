// kaldifst/python/csrc/fstext-utils.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/csrc/fstext-utils.h"

#include "kaldifst/python/csrc/fstarcsort.h"

namespace kaldifst {

void PybindFstExtUtils(py::module &m) {
  m.def(
      "minimize_encoded",
      [](fst::MutableFst<fst::StdArc> *in_out, double delta = fst::kDelta) {
        fst::MinimizeEncoded(in_out, delta);
      },
      py::arg("in_out"), py::arg("delta") = fst::kDelta);
}

}  // namespace kaldifst
