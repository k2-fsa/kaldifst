// kaldifst/python/csrc/mutable-fst.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/mutable-fst.h"

namespace kaldifst {

void PybindMutableFst(py::module &m) {  // NOLINT
  PybindMutableFst<fst::StdArc>(m, "StdMutableFst");
}

}  // namespace kaldifst
