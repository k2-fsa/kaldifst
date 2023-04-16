// kaldifst/csrc/fstext-utils-inl.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/fstext-utils-inl.h

// Copyright 2009-2012  Microsoft Corporation  Johns Hopkins University
//                      (Author: Daniel Povey)
//
//           2014  Telepoint Global Hosting Service, LLC.
//                 (Author: David Snyder)

#ifndef KALDIFST_CSRC_FSTEXT_UTILS_INL_H_
#define KALDIFST_CSRC_FSTEXT_UTILS_INL_H_

#include "kaldifst/csrc/determinize-star.h"

namespace fst {

inline bool DeterminizeStarInLog(VectorFst<StdArc> *fst, float delta,
                                 bool *debug_ptr, int max_states) {
  // DeterminizeStarInLog determinizes 'fst' in the log semiring, using
  // the DeterminizeStar algorithm (which also removes epsilons).

  ArcSort(fst, ILabelCompare<StdArc>());  // helps DeterminizeStar to be faster.
  VectorFst<LogArc> *fst_log =
      new VectorFst<LogArc>;  // Want to determinize in log semiring.
  Cast(*fst, fst_log);
  VectorFst<StdArc> tmp;
  *fst = tmp;  // make fst empty to free up memory. [actually may make no
               // difference..]
  VectorFst<LogArc> *fst_det_log = new VectorFst<LogArc>;
  bool status =
      DeterminizeStar(*fst_log, fst_det_log, delta, debug_ptr, max_states);
  Cast(*fst_det_log, fst);
  delete fst_log;
  delete fst_det_log;
  return status;
}

template <class Arc, class I>
void MakeLinearAcceptor(const std::vector<I> &labels, MutableFst<Arc> *ofst) {
  typedef typename Arc::StateId StateId;
  typedef typename Arc::Weight Weight;

  ofst->DeleteStates();
  StateId cur_state = ofst->AddState();
  ofst->SetStart(cur_state);
  for (size_t i = 0; i < labels.size(); i++) {
    StateId next_state = ofst->AddState();
    Arc arc(labels[i], labels[i], Weight::One(), next_state);
    ofst->AddArc(cur_state, arc);
    cur_state = next_state;
  }
  ofst->SetFinal(cur_state, Weight::One());
}

}  // namespace fst
#endif  // KALDIFST_CSRC_FSTEXT_UTILS_INL_H_
