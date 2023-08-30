// kaldifst/csrc/pre-determinize.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/pre-determinize.h

// Copyright 2009-2011  Microsoft Corporation

#ifndef KALDIFST_CSRC_PRE_DETERMINIZE_H_
#define KALDIFST_CSRC_PRE_DETERMINIZE_H_

#include <algorithm>
#include <vector>

#include "fst/fst-decl.h"
#include "fst/fstlib.h"

namespace fst {

/** AddSelfLoops is a function you will probably want to use alongside
   PreDeterminize, to add self-loops to any FSTs that you compose on the left
   hand side of the one modified by PreDeterminize.

    This function inserts loops with "special symbols" [e.g. \#0, \#1] into an
   FST. This is done at each final state and each state with non-epsilon output
   symbols on at least one arc out of it.  This is to ensure that these symbols,
   when inserted into the input side of an FST we will compose with on the
   right, can "pass through" this FST.

    At input, isyms and osyms must be vectors of the same size n, corresponding
    to symbols that currently do not exist in 'fst'.  For each state in n that
   has non-epsilon symbols on the output side of arcs leaving it, or which is a
   final state, this function inserts n self-loops with unit weight and one of
   the n pairs of symbols on its input and output.
*/
template <class Arc>
void AddSelfLoops(MutableFst<Arc> *fst,
                  const std::vector<typename Arc::Label> &isyms,
                  const std::vector<typename Arc::Label> &osyms);

}  // namespace fst

#include "kaldifst/csrc/pre-determinize-inl.h"

#endif  // KALDIFST_CSRC_PRE_DETERMINIZE_H_
