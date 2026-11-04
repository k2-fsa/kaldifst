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

  // ---- Cross-module zero-copy helpers ---------------------------------
  // Each pybind11 module bundles its own hidden copy of openfst, so
  // std::type_index based cross-module type matching cannot work.  These
  // helpers expose the raw C++ address of a VectorFst and can wrap an
  // arbitrary address back as a (non-)owning StdVectorFst, letting other
  // pybind11 modules (kaldi-decoder, kaldi-hmm-gmm, ...) share the object by
  // pointer with zero copies.  Contract: all modules must be built against
  // the same openfst version and ABI-relevant flags.
  m.def(
      "_vector_fst_addr",
      [](const fst::VectorFst<fst::StdArc> &f) -> uint64_t {
        return reinterpret_cast<uint64_t>(&f);
      },
      py::arg("fst"), "Returns the raw C++ address of the given StdVectorFst.");
  m.def(
      "_vector_fst_borrow",
      [](uint64_t addr) -> fst::VectorFst<fst::StdArc> * {
        return reinterpret_cast<fst::VectorFst<fst::StdArc> *>(addr);
      },
      py::return_value_policy::reference,
      "Wraps the given address as a NON-owning StdVectorFst (the caller keeps "
      "ownership).");
  m.def(
      "_vector_fst_take",
      [](uint64_t addr) -> fst::VectorFst<fst::StdArc> * {
        return reinterpret_cast<fst::VectorFst<fst::StdArc> *>(addr);
      },
      py::return_value_policy::take_ownership,
      "Wraps the given address as an OWNING StdVectorFst (must have been "
      "allocated with new by the caller).");
}

}  // namespace kaldifst
