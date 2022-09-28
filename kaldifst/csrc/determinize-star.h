// kaldifst/csrc/determinize-star.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/determinize-star.h

// Copyright 2009-2011  Microsoft Corporation
//                2014  Guoguo Chen
//                2015  Hainan Xu

#ifndef KALDIFST_CSRC_DETERMINIZE_STAR_H_
#define KALDIFST_CSRC_DETERMINIZE_STAR_H_

#include "fst/fst-decl.h"
#include "fst/fst.h"

namespace fst {

// This algorithm will be slightly faster if you sort the input fst on input
// label.

/**
    This function implements the normal version of DeterminizeStar, in which the
    output strings are represented using sequences of arcs, where all but the
    first one has an epsilon on the input side.  The debug_ptr argument is an
    optional pointer to a bool that, if it becomes true while the algorithm is
    executing, the algorithm will print a traceback and terminate (used in
    fstdeterminizestar.cc debug non-terminating determinization).
    If max_states is positive, it will stop determinization and throw an
    exception as soon as the max-states is reached. This can be useful in test.
    If allow_partial is true, the algorithm will output partial results when the
    specified max_states is reached (when larger than zero), instead of throwing
    out an error.

    Caution, the return status is un-intuitive: this function will return false
    if determinization completed normally, and true if it was stopped early by
    reaching the 'max-states' limit, and a partial FST was generated.
*/
template <class F>
bool DeterminizeStar(F &ifst,  // NOLINT
                     MutableFst<typename F::Arc> *ofst, float delta = kDelta,
                     bool *debug_ptr = NULL, int max_states = -1,
                     bool allow_partial = false);
}  // namespace fst

#include "kaldifst/csrc/determinize-star-inl.h"

#endif  // KALDIFST_CSRC_DETERMINIZE_STAR_H_
