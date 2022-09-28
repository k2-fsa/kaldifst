// kaldifst/python/csrc/fstinfo.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstinfo.h"

namespace kaldifst {

void PybindFstInfo(py::module &m) {  // NOLINT
  PybindFstInfo<fst::StdArc>(m);
}

}  // namespace kaldifst
