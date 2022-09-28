// kaldifst/python/csrc/vector-fst.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/vector-fst.h"

#include "kaldifst/python/csrc/expanded-fst.h"
#include "kaldifst/python/csrc/fst.h"
#include "kaldifst/python/csrc/mutable-fst.h"

namespace kaldifst {

void PybindVectorFst(py::module &m) {  // NOLINT
  PybindVectorState<fst::StdArc>(m, "_StdVectorState");
  PybindVectorFstBaseImpl<fst::VectorState<fst::StdArc>>(
      m, "_StdVectorFstBaseImpl");
  PybindVectorFstImpl<fst::VectorState<fst::StdArc>>(m, "_StdVectorFstImpl");

  PybindImplToFst<fst::internal::VectorFstImpl<fst::VectorState<fst::StdArc>>,
                  fst::MutableFst<fst::StdArc>>(m, "_StdImplToFst");

  PybindImplToExpandedFst<
      fst::internal::VectorFstImpl<fst::VectorState<fst::StdArc>>,
      fst::MutableFst<fst::StdArc>>(m, "_StdImplToExpandedFst");

  PybindImplToMutableFst<
      fst::internal::VectorFstImpl<fst::VectorState<fst::StdArc>>>(
      m, "_StdImplToMutableFst");

  PybindVectorFst<fst::StdArc>(m, "StdVectorFst");
  PybindStateIteratorVectorFst<fst::StdArc, fst::VectorState<fst::StdArc>>(
      m, "_StateIteratorStdVectorFst");
  PybindArcIteratorVectorFst<fst::StdArc, fst::VectorState<fst::StdArc>>(
      m, "_ArcIteratorStdVectorFst");
}

}  // namespace kaldifst
