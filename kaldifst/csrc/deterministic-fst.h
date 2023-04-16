// kaldifst/csrc/deterministic-fst.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/deterministic-fst.h

// Copyright 2011-2012 Gilles Boulianne
//                2014 Telepoint Global Hosting Service, LLC. (Author: David
//                Snyder)
//           2012-2015 Johns Hopkins University (author: Daniel Povey)
#ifndef KALDIFST_CSRC_DETERMINISTIC_FST_H_
#define KALDIFST_CSRC_DETERMINISTIC_FST_H_

/* This header defines the DeterministicOnDemand interface,
   which is an FST with a special interface that allows
   only a single arc with a non-epsilon input symbol
   out of each state.
*/

#include <unordered_map>
#include <utility>
#include <vector>

#include "fst/fst-decl.h"
#include "fst/fstlib.h"

namespace fst {

/// \addtogroup deterministic_fst_group "Classes and functions related to
/// on-demand deterministic FST's"
/// @{

/// class DeterministicOnDemandFst is an "FST-like" base-class.  It does not
/// actually inherit from any Fst class because its interface is not exactly the
/// same; it's much smaller.  It assumes that the FST can have only one arc for
/// any given input symbol, which makes the GetArc function below possible.
/// (The FST is also assumed to be free of input epsilons).  Note: we don't use
/// "const" in this interface, because it creates problems when we do things
/// like caching.
template <class Arc>
class DeterministicOnDemandFst {
 public:
  typedef typename Arc::StateId StateId;
  typedef typename Arc::Weight Weight;
  typedef typename Arc::Label Label;

  virtual StateId Start() = 0;

  virtual Weight Final(StateId s) = 0;

  /// Note: ilabel must not be epsilon.
  virtual bool GetArc(StateId s, Label ilabel, Arc *oarc) = 0;

  virtual ~DeterministicOnDemandFst() = default;
};

template <class Arc>
class ComposeDeterministicOnDemandFst : public DeterministicOnDemandFst<Arc> {
 public:
  typedef typename Arc::StateId StateId;
  typedef typename Arc::Weight Weight;
  typedef typename Arc::Label Label;

  /// Note: constructor does not "take ownership" of the input fst's.  The input
  /// fst's should be treated as const, in that their contents do not change,
  /// but they are not const as the DeterministicOnDemandFst's data-access
  /// functions are not const, for reasons relating to caching.
  ComposeDeterministicOnDemandFst(DeterministicOnDemandFst<Arc> *fst1,
                                  DeterministicOnDemandFst<Arc> *fst2);

  StateId Start() override { return start_state_; }

  Weight Final(StateId s) override;

  bool GetArc(StateId s, Label ilabel, Arc *oarc) override;

 private:
  DeterministicOnDemandFst<Arc> *fst1_;
  DeterministicOnDemandFst<Arc> *fst2_;
  typedef unordered_map<std::pair<StateId, StateId>, StateId,
                        kaldifst::PairHasher<StateId>>
      MapType;
  MapType state_map_;
  std::vector<std::pair<StateId, StateId>> state_vec_;  // maps from
  // StateId to pair.
  StateId next_state_;
  StateId start_state_;
};

// Compose an FST (which may be a lattice) with a DeterministicOnDemandFst and
// store the result in fst_composed.  This is mainly used for expanding lattice
// n-gram histories, where fst1 is a lattice and fst2 is an UnweightedNgramFst.
// This does not call Connect.
template <class Arc>
void ComposeDeterministicOnDemand(const Fst<Arc> &fst1,
                                  DeterministicOnDemandFst<Arc> *fst2,
                                  MutableFst<Arc> *fst_composed);

/**
   This function does
   '*fst_composed = Compose(Inverse(*fst2), fst1)'
   Note that the arguments are reversed; this is unfortunate but it's
   because the fst2 argument needs to be non-const and non-const arguments
   must follow const ones.
   This is the counterpart to ComposeDeterministicOnDemand, used for
   the case where the DeterministicOnDemandFst is on the left.  The
   reason why we need to make the left-hand argument to compose the
   inverse of 'fst2' (i.e. with the input and output symbols swapped),
   is that the DeterministicOnDemandFst interface only supports lookup
   by ilabel (see its function GetArc).
   This does not call Connect().
*/
template <class Arc>
void ComposeDeterministicOnDemandInverse(const Fst<Arc> &fst1,
                                         DeterministicOnDemandFst<Arc> *fst2,
                                         MutableFst<Arc> *fst_composed);

}  // namespace fst

#include "kaldifst/csrc/deterministic-fst-inl.h"

#endif  // KALDIFST_CSRC_DETERMINISTIC_FST_H_
