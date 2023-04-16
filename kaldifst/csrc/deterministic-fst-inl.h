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
#include <algorithm>
#include <queue>

#include "kaldifst/csrc/log.h"
#include "kaldifst/csrc/stl-utils.h"

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

template <class Arc>
void ComposeDeterministicOnDemand(const Fst<Arc> &fst1,
                                  DeterministicOnDemandFst<Arc> *fst2,
                                  MutableFst<Arc> *fst_composed) {
  typedef typename Arc::Weight Weight;
  typedef typename Arc::StateId StateId;
  typedef std::pair<StateId, StateId> StatePair;
  typedef unordered_map<StatePair, StateId, kaldifst::PairHasher<StateId>>
      MapType;
  typedef typename MapType::iterator IterType;

  fst_composed->DeleteStates();

  MapType state_map;
  std::queue<StatePair> state_queue;

  // Set start state in fst_composed.
  StateId s1 = fst1.Start(), s2 = fst2->Start(),
          start_state = fst_composed->AddState();
  StatePair start_pair(s1, s2);
  state_queue.push(start_pair);
  fst_composed->SetStart(start_state);
  // A mapping between pairs of states in fst1 and fst2 and the corresponding
  // state in fst_composed.
  std::pair<const StatePair, StateId> start_map(start_pair, start_state);
  std::pair<IterType, bool> result = state_map.insert(start_map);
  KALDIFST_ASSERT(result.second == true);

  while (!state_queue.empty()) {
    StatePair q = state_queue.front();
    StateId q1 = q.first, q2 = q.second;
    state_queue.pop();
    // If the product of the final weights of the two fsts is non-zero then
    // we can set a final-prob in fst_composed
    Weight final_weight = Times(fst1.Final(q1), fst2->Final(q2));
    if (final_weight != Weight::Zero()) {
      KALDIFST_ASSERT(state_map.find(q) != state_map.end());
      fst_composed->SetFinal(state_map[q], final_weight);
    }

    // for each pair of edges from fst1 and fst2 at q1 and q2.
    for (ArcIterator<Fst<Arc>> aiter(fst1, q1); !aiter.Done(); aiter.Next()) {
      const Arc &arc1 = aiter.Value();
      Arc arc2;
      StatePair next_pair;
      StateId next_state1 = arc1.nextstate, next_state2, next_state;
      // If there is an epsilon on the arc of fst1 we transition to the next
      // state but keep fst2 at the current state.
      if (arc1.olabel == 0) {
        next_state2 = q2;
      } else {
        bool match = fst2->GetArc(q2, arc1.olabel, &arc2);
        if (!match)  // There is no matching arc -> nothing to do.
          continue;
        next_state2 = arc2.nextstate;
      }
      next_pair = StatePair(next_state1, next_state2);
      IterType sitr = state_map.find(next_pair);
      // If sitr == state_map.end() then the state isn't in fst_composed yet.
      if (sitr == state_map.end()) {
        next_state = fst_composed->AddState();
        std::pair<const StatePair, StateId> new_state(next_pair, next_state);
        std::pair<IterType, bool> result = state_map.insert(new_state);
        // Since we already checked if state_map contained new_state,
        // it should always be added if we reach here.
        KALDIFST_ASSERT(result.second == true);
        state_queue.push(next_pair);
        // If sitr != state_map.end() then the next state is already in
        // the state_map.
      } else {
        next_state = sitr->second;
      }
      if (arc1.olabel == 0) {
        fst_composed->AddArc(state_map[q],
                             Arc(arc1.ilabel, 0, arc1.weight, next_state));
      } else {
        fst_composed->AddArc(state_map[q],
                             Arc(arc1.ilabel, arc2.olabel,
                                 Times(arc1.weight, arc2.weight), next_state));
      }
    }
  }
}

// we are doing *fst_composed = Compose(Inverse(*left), right).
template <class Arc>
void ComposeDeterministicOnDemandInverse(const Fst<Arc> &right,
                                         DeterministicOnDemandFst<Arc> *left,
                                         MutableFst<Arc> *fst_composed) {
  typedef typename Arc::Weight Weight;
  typedef typename Arc::StateId StateId;
  typedef std::pair<StateId, StateId> StatePair;
  typedef unordered_map<StatePair, StateId, kaldifst::PairHasher<StateId>>
      MapType;
  typedef typename MapType::iterator IterType;

  fst_composed->DeleteStates();

  // the queue and map contain pairs (state-in-left, state-in-right)
  MapType state_map;
  std::queue<StatePair> state_queue;

  // Set start state in fst_composed.
  StateId s_left = left->Start(), s_right = right.Start();
  if (s_left == kNoStateId || s_right == kNoStateId) return;  // Empty result.
  StatePair start_pair(s_left, s_right);
  StateId start_state = fst_composed->AddState();
  state_queue.push(start_pair);
  fst_composed->SetStart(start_state);
  // A mapping between pairs of states in *left and right, and the corresponding
  // state in fst_composed.
  std::pair<const StatePair, StateId> start_map(start_pair, start_state);
  std::pair<IterType, bool> result = state_map.insert(start_map);
  KALDIFST_ASSERT(result.second == true);

  while (!state_queue.empty()) {
    StatePair q = state_queue.front();
    StateId q_left = q.first, q_right = q.second;
    state_queue.pop();
    // If the product of the final weights of the two fsts is non-zero then
    // we can set a final-prob in fst_composed
    Weight final_weight = Times(left->Final(q_left), right.Final(q_right));
    if (final_weight != Weight::Zero()) {
      KALDIFST_ASSERT(state_map.find(q) != state_map.end());
      fst_composed->SetFinal(state_map[q], final_weight);
    }

    for (ArcIterator<Fst<Arc>> aiter(right, q_right); !aiter.Done();
         aiter.Next()) {
      const Arc &arc_right = aiter.Value();
      Arc arc_left;
      StatePair next_pair;
      StateId next_state_right = arc_right.nextstate, next_state_left,
              next_state;
      // If there is an epsilon on the input side of the rigth arc, we
      // transition to the next state of the output but keep 'left' at the
      // current state.
      if (arc_right.ilabel == 0) {
        next_state_left = q_left;
      } else {
        bool match = left->GetArc(q_left, arc_right.ilabel, &arc_left);
        if (!match)  // There is no matching arc -> nothing to do.
          continue;
        // the next 'swap' is because we are composing with the inverse of
        // *left.  Just removing the swap statement wouldn't let us compose
        // with non-inverted *left though, because the GetArc function call
        // above interprets the second argument as an ilabel not an olabel.
        std::swap(arc_left.ilabel, arc_left.olabel);
        next_state_left = arc_left.nextstate;
      }
      next_pair = StatePair(next_state_left, next_state_right);
      IterType sitr = state_map.find(next_pair);
      // If sitr == state_map.end() then the state isn't in fst_composed yet.
      if (sitr == state_map.end()) {
        next_state = fst_composed->AddState();
        std::pair<const StatePair, StateId> new_state(next_pair, next_state);
        std::pair<IterType, bool> result = state_map.insert(new_state);
        // Since we already checked if state_map contained new_state,
        // it should always be added if we reach here.
        KALDIFST_ASSERT(result.second == true);
        state_queue.push(next_pair);
        // If sitr != state_map.end() then the next state is already in
        // the state_map.
      } else {
        next_state = sitr->second;
      }
      if (arc_right.ilabel == 0) {
        // we didn't get an actual arc from the left FST.
        fst_composed->AddArc(state_map[q], Arc(0, arc_right.olabel,
                                               arc_right.weight, next_state));
      } else {
        fst_composed->AddArc(
            state_map[q],
            Arc(arc_left.ilabel, arc_right.olabel,
                Times(arc_left.weight, arc_right.weight), next_state));
      }
    }
  }
}

}  // namespace fst

#endif  // KALDIFST_CSRC_DETERMINISTIC_FST_INL_H_
