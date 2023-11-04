// kaldifst/csrc/lattice-utils.h
//
// Copyright 2009-2011  Microsoft Corporation
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/lattice-utils.h

#ifndef KALDIFST_CSRC_LATTICE_UTILS_H_
#define KALDIFST_CSRC_LATTICE_UTILS_H_

#include <vector>

#include "fst/fstlib.h"
#include "kaldifst/csrc/lattice-weight.h"

namespace fst {
/** Returns a default 2x2 matrix scaling factor for LatticeWeight */
inline std::vector<std::vector<double>> DefaultLatticeScale() {
  std::vector<std::vector<double>> ans(2);
  ans[0].resize(2, 0.0);
  ans[1].resize(2, 0.0);
  ans[0][0] = ans[1][1] = 1.0;
  return ans;
}

inline std::vector<std::vector<double>> LatticeScale(double lmwt, double acwt) {
  std::vector<std::vector<double>> ans(2);
  ans[0].resize(2, 0.0);
  ans[1].resize(2, 0.0);
  ans[0][0] = lmwt;
  ans[1][1] = acwt;
  return ans;
}

/** Scales the pairs of weights in LatticeWeight or CompactLatticeWeight by
    viewing the pair (a, b) as a 2-vector and pre-multiplying by the 2x2 matrix
    in "scale".  E.g. typically scale would equal
     [ 1   0;
       0  acwt ]
    if we want to scale the acoustics by "acwt".
 */
template <class Weight, class ScaleFloat>
void ScaleLattice(const std::vector<std::vector<ScaleFloat>> &scale,
                  MutableFst<ArcTpl<Weight>> *fst);

}  // namespace fst

#include "kaldifst/csrc/lattice-utils-inl.h"

#endif  // KALDIFST_CSRC_LATTICE_UTILS_H_
