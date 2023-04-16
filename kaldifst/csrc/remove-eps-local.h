// kaldifst/csrc/remove-eps-local.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/remove-eps-local.h

// Copyright 2009-2011  Microsoft Corporation
//                2014  Johns Hopkins University (author: Daniel Povey)

#ifndef KALDIFST_CSRC_REMOVE_EPS_LOCAL_H_
#define KALDIFST_CSRC_REMOVE_EPS_LOCAL_H_

#include "fst/fst-decl.h"
#include "fst/fstlib.h"

namespace fst {

/// RemoveEpsLocal remove some (but not necessarily all) epsilons in an FST,
/// using an algorithm that is guaranteed to never increase the number of arcs
/// in the FST (and will also never increase the number of states).  The
/// algorithm is not optimal but is reasonably clever.  It does not just remove
/// epsilon arcs;it also combines pairs of input-epsilon and output-epsilon arcs
/// into one.
/// The algorithm preserves equivalence and stochasticity in the given semiring.
/// If you want to preserve stochasticity in a different semiring (e.g. log),
/// then use RemoveEpsLocalSpecial, which only works for StdArc but which
/// preserves stochasticity, where possible (*) in the LogArc sense.  The reason
/// that we can't just cast to a different semiring is that in that case we
/// would no longer be able to guarantee equivalence in the original semiring
/// (this arises from what happens when we combine identical arcs).
/// (*) by "where possible".. there are situations where we wouldn't be able to
/// preserve stochasticity in the LogArc sense while maintaining equivalence in
/// the StdArc sense, so in these situations we maintain equivalence.

template <class Arc>
void RemoveEpsLocal(MutableFst<Arc> *fst);

/// As RemoveEpsLocal but takes care to preserve stochasticity
/// when cast to LogArc.
inline void RemoveEpsLocalSpecial(MutableFst<StdArc> *fst);

}  // namespace fst

#include "kaldifst/csrc/remove-eps-local-inl.h"

#endif  //  KALDIFST_CSRC_REMOVE_EPS_LOCAL_H_
