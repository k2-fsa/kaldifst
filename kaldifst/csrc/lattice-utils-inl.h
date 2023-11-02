// kaldifst/csrc/lattice-utils-inl.h
//
// Copyright 2009-2011  Microsoft Corporation
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/lattice-utils-inl.h
#ifndef KALDIFST_CSRC_LATTICE_UTILS_INL_H_
#define KALDIFST_CSRC_LATTICE_UTILS_INL_H_

#include <assert.h>

namespace fst {

template <class Weight, class ScaleFloat>
void ScaleLattice(const std::vector<std::vector<ScaleFloat>> &scale,
                  MutableFst<ArcTpl<Weight>> *fst) {
  assert(scale.size() == 2 && scale[0].size() == 2 && scale[1].size() == 2);
  if (scale == DefaultLatticeScale()) {  // nothing to do.
    return;
  }

  using Arc = ArcTpl<Weight>;
  using Fst = MutableFst<Arc>;
  using StateId = typename Arc::StateId;

  StateId num_states = fst->NumStates();
  for (StateId s = 0; s < num_states; s++) {
    for (MutableArcIterator<Fst> aiter(fst, s); !aiter.Done(); aiter.Next()) {
      Arc arc = aiter.Value();
      arc.weight = Weight(ScaleTupleWeight(arc.weight, scale));
      aiter.SetValue(arc);
    }
    Weight final_weight = fst->Final(s);
    if (final_weight != Weight::Zero())
      fst->SetFinal(s, Weight(ScaleTupleWeight(final_weight, scale)));
  }
}

}  // namespace fst
#endif  // KALDIFST_CSRC_LATTICE_UTILS_INL_H_
