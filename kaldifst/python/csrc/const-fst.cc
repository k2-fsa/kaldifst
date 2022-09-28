// kaldifst/python/csrc/const-fst.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/const-fst.h"

#include "kaldifst/python/csrc/expanded-fst.h"
#include "kaldifst/python/csrc/fst.h"

namespace kaldifst {

void PybindConstFst(py::module &m) {  // NOLINT
  PybindConstFstImpl<fst::StdArc>(m, "_ConstFstImpl");

  PybindImplToFst<fst::internal::ConstFstImpl<fst::StdArc, uint32_t>,
                  fst::ExpandedFst<fst::StdArc>>(m, "_ConstImplToFst");

  PybindImplToExpandedFst<fst::internal::ConstFstImpl<fst::StdArc, uint32_t>>(
      m, "_ConstImplToExpandedFst");
  PybindConstFst<fst::StdArc>(m, "StdConstFst");

  PybindStateIteratorConstFst<fst::StdArc, uint32_t>(
      m, "_StateIteratorStdConstFst");

  PybindArcIteratorConstFst<fst::StdArc, uint32_t>(m,
                                                   "_ArcIteratorStdConstFst");
}

}  // namespace kaldifst
