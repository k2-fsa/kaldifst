// kaldifst/python/csrc/fstinvert.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstinvert.h"

#include "fst/invert.h"

namespace kaldifst {

void PybindFstInvert(py::module &m) {  // NOLINT
  m.def("invert",                      // in-place
        [](fst::MutableFst<fst::StdArc> *in_out) { fst::Invert(in_out); });
}

}  // namespace kaldifst
