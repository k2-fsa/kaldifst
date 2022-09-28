// kaldifst/python/csrc/expanded-fst.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "fst/expanded-fst.h"

#include "kaldifst/python/csrc/expanded-fst.h"

namespace kaldifst {

void PybindExpandedFst(py::module &m) {  // NOLINT
  PybindExpandedFst<fst::StdArc>(m, "StdExpandedFst");
  //
}

}  // namespace kaldifst
