// kaldifst/csrc/context-fst.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/context-fst.h

// Copyright 2009-2011  Microsoft Corporation
//                2018  Johns Hopkins University (author: Daniel Povey)
#ifndef KALDIFST_CSRC_CONTEXT_FST_H_
#define KALDIFST_CSRC_CONTEXT_FST_H_

#include "fst/fst-decl.h"
#include "fst/fstlib.h"

namespace fst {

/**
  Modifies an FST so that it transuces the same paths, but the input side of the
  paths can all have the subsequential symbol '$' appended to them any number of
  times (we could easily specify the number of times, but accepting any number
  of repetitions is just more convenient).  The actual way we do this is for
  each final state, we add a transition with weight equal to the final-weight of
  that state, with input-symbol '$' and output-symbols \<eps\>, and ending in a
  new super-final state that has unit final-probability and a unit-weight
  self-loop with '$' on its input and \<eps\> on its output.  The reason we
  don't just add a loop to each final-state has to do with preserving
  stochasticity (see \ref fst_algo_stochastic).  We keep the final-probability
  in all the original final-states rather than setting them to zero, so the
  resulting FST can accept zero '$' symbols at the end (in case we had no right
  context).
*/
void AddSubsequentialLoop(StdArc::Label subseq_symbol, MutableFst<StdArc> *fst);

}  // namespace fst

#endif  // KALDIFST_CSRC_CONTEXT_FST_H_
