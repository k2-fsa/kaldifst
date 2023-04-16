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

}  // namespace fst

#include "kaldifst/csrc/deterministic-fst-inl.h"

#endif  // KALDIFST_CSRC_DETERMINISTIC_FST_H_
