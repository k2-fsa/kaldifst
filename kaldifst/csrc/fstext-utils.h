// kaldifst/csrc/fstext-utils.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/fstext-utils.h

// Copyright 2009-2011  Microsoft Corporation
//           2012-2013  Johns Hopkins University (Author: Daniel Povey)
//                2013  Guoguo Chen
//                2014  Telepoint Global Hosting Service, LLC. (Author: David
//                Snyder)

#ifndef KALDIFST_CSRC_KALDI_FSTEXT_UTILS_H_
#define KALDIFST_CSRC_KALDI_FSTEXT_UTILS_H_

#include "fst/fst.h"
#include "fst/map.h"
#include "fst/minimize.h"

namespace fst {

// Minimizes after encoding; applicable to all FSTs.  It is like what you get
// from the Minimize() function, except it will not push the weights, or the
// symbols.  This is better for our recipes, as we avoid ever pushing the
// weights.  However, it will only minimize optimally if your graphs are such
// that the symbols are as far to the left as they can go, and the weights
// in combinable paths are the same... hard to formalize this, but it's
// something that is satisfied by our normal FSTs.
template <class Arc>
void MinimizeEncoded(MutableFst<Arc> *fst, float delta = kDelta) {
  Map(fst, QuantizeMapper<Arc>(delta));
  EncodeMapper<Arc> encoder(kEncodeLabels | kEncodeWeights, ENCODE);
  Encode(fst, &encoder);
  internal::AcceptorMinimize(fst);
  Decode(fst, encoder);
}

}  // namespace fst

#endif  // KALDIFST_CSRC_KALDI_FSTEXT_UTILS_H_
