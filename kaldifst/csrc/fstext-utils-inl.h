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
}  // namespace fst
#endif  // KALDIFST_CSRC_FSTEXT_UTILS_INL_H_
