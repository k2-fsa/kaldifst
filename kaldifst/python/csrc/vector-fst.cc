// kaldifst/python/csrc/vector-fst.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/vector-fst.h"

#include "kaldifst/csrc/lattice-weight.h"
#include "kaldifst/python/csrc/expanded-fst.h"
#include "kaldifst/python/csrc/fst.h"
#include "kaldifst/python/csrc/mutable-fst.h"

namespace kaldifst {

static void PybindStdVectorFst(py::module &m) {  // NOLINT
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

static void PybindLattice(py::module &m) {  // NOLINT
  PybindVectorState<fst::LatticeArc>(m, "_LatticeVectorState");
  PybindVectorFstBaseImpl<fst::VectorState<fst::LatticeArc>>(
      m, "_LatticeVectorFstBaseImpl");
  PybindVectorFstImpl<fst::VectorState<fst::LatticeArc>>(
      m, "_LatticeVectorFstImpl");

  PybindImplToFst<
      fst::internal::VectorFstImpl<fst::VectorState<fst::LatticeArc>>,
      fst::MutableFst<fst::LatticeArc>>(m, "_LatticeImplToFst");

  PybindImplToExpandedFst<
      fst::internal::VectorFstImpl<fst::VectorState<fst::LatticeArc>>,
      fst::MutableFst<fst::LatticeArc>>(m, "_LatticeImplToExpandedFst");

  PybindImplToMutableFst<
      fst::internal::VectorFstImpl<fst::VectorState<fst::LatticeArc>>>(
      m, "_LatticeImplToMutableFst");

  PybindVectorFst<fst::LatticeArc>(m, "Lattice");
  PybindStateIteratorVectorFst<fst::LatticeArc,
                               fst::VectorState<fst::LatticeArc>>(
      m, "_StateIteratorLattice");
  PybindArcIteratorVectorFst<fst::LatticeArc,
                             fst::VectorState<fst::LatticeArc>>(
      m, "_ArcIteratorLattice");
}

void PybindVectorFst(py::module &m) {  // NOLINT
  PybindStdVectorFst(m);
  PybindLattice(m);
}

}  // namespace kaldifst
