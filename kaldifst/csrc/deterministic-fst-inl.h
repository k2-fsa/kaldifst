// kaldifst/csrc/deterministic-fst-inl.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/deterministic-fst-inl.h

// Copyright 2011-2012 Gilles Boulianne
//                2014 Telepoint Global Hosting Service, LLC. (Author: David
//                Snyder)
//           2012-2015 Johns Hopkins University (author: Daniel Povey)
#ifndef KALDIFST_CSRC_DETERMINISTIC_FST_INL_H_
#define KALDIFST_CSRC_DETERMINISTIC_FST_INL_H_

namespace fst {

template <class Arc>
ComposeDeterministicOnDemandFst<Arc>::ComposeDeterministicOnDemandFst(
    DeterministicOnDemandFst<Arc> *fst1, DeterministicOnDemandFst<Arc> *fst2)
    : fst1_(fst1), fst2_(fst2) {
  KALDIFST_ASSERT(fst1 != NULL && fst2 != NULL);
  if (fst1_->Start() == -1 || fst2_->Start() == -1) {
    start_state_ = -1;
    next_state_ = 0;  // actually we don't care about this value.
  } else {
    start_state_ = 0;
    std::pair<StateId, StateId> start_pair(fst1_->Start(), fst2_->Start());
    state_map_[start_pair] = start_state_;
    state_vec_.push_back(start_pair);
    next_state_ = 1;
  }
}

template <class Arc>
typename Arc::Weight ComposeDeterministicOnDemandFst<Arc>::Final(StateId s) {
  KALDIFST_ASSERT(s < static_cast<StateId>(state_vec_.size()));
  const std::pair<StateId, StateId> &pr(state_vec_[s]);
  return Times(fst1_->Final(pr.first), fst2_->Final(pr.second));
}

template <class Arc>
bool ComposeDeterministicOnDemandFst<Arc>::GetArc(StateId s, Label ilabel,
                                                  Arc *oarc) {
  typedef typename MapType::iterator IterType;
  KALDIFST_ASSERT(
      ilabel != 0 &&
      "This program expects epsilon-free compact lattices as input");
  KALDIFST_ASSERT(s < static_cast<StateId>(state_vec_.size()));
  const std::pair<StateId, StateId> pr(state_vec_[s]);

  Arc arc1;
  if (!fst1_->GetArc(pr.first, ilabel, &arc1)) return false;
  if (arc1.olabel == 0) {  // There is no output label on the
    // arc, so only the first state changes.
    std::pair<const std::pair<StateId, StateId>, StateId> new_value(
        std::pair<StateId, StateId>(arc1.nextstate, pr.second), next_state_);

    std::pair<IterType, bool> result = state_map_.insert(new_value);
    oarc->ilabel = ilabel;
    oarc->olabel = 0;
    oarc->nextstate = result.first->second;
    oarc->weight = arc1.weight;
    if (result.second == true) {  // was inserted
      next_state_++;
      const std::pair<StateId, StateId> &new_pair(new_value.first);
      state_vec_.push_back(new_pair);
    }
    return true;
  }
  // There is an output label, so we need to traverse an arc on the
  // second fst also.
  Arc arc2;
  if (!fst2_->GetArc(pr.second, arc1.olabel, &arc2)) return false;
  std::pair<const std::pair<StateId, StateId>, StateId> new_value(
      std::pair<StateId, StateId>(arc1.nextstate, arc2.nextstate), next_state_);
  std::pair<IterType, bool> result = state_map_.insert(new_value);
  oarc->ilabel = ilabel;
  oarc->olabel = arc2.olabel;
  oarc->nextstate = result.first->second;
  oarc->weight = Times(arc1.weight, arc2.weight);
  if (result.second == true) {  // was inserted
    next_state_++;
    const std::pair<StateId, StateId> &new_pair(new_value.first);
    state_vec_.push_back(new_pair);
  }
  return true;
}

}  // namespace fst

#endif  // KALDIFST_CSRC_DETERMINISTIC_FST_INL_H_
