// kaldifst/csrc/fstext-utils-inl.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/fstext-utils-inl.h

// Copyright 2009-2012  Microsoft Corporation  Johns Hopkins University
//                      (Author: Daniel Povey)
//
//           2014  Telepoint Global Hosting Service, LLC.
//                 (Author: David Snyder)

#ifndef KALDIFST_CSRC_FSTEXT_UTILS_INL_H_
#define KALDIFST_CSRC_FSTEXT_UTILS_INL_H_

#include <type_traits>
#include <unordered_map>

#include "kaldifst/csrc/const-integer-set.h"
#include "kaldifst/csrc/determinize-star.h"

namespace fst {

inline bool DeterminizeStarInLog(VectorFst<StdArc> *fst, float delta,
                                 bool *debug_ptr, int max_states) {
  // DeterminizeStarInLog determinizes 'fst' in the log semiring, using
  // the DeterminizeStar algorithm (which also removes epsilons).

  ArcSort(fst, ILabelCompare<StdArc>());  // helps DeterminizeStar to be faster.
  VectorFst<LogArc> *fst_log =
      new VectorFst<LogArc>;  // Want to determinize in log semiring.
  Cast(*fst, fst_log);
  VectorFst<StdArc> tmp;
  *fst = tmp;  // make fst empty to free up memory. [actually may make no
               // difference..]
  VectorFst<LogArc> *fst_det_log = new VectorFst<LogArc>;
  bool status =
      DeterminizeStar(*fst_log, fst_det_log, delta, debug_ptr, max_states);
  Cast(*fst_det_log, fst);
  delete fst_log;
  delete fst_det_log;
  return status;
}

template <class Arc, class I>
void MakeLinearAcceptor(const std::vector<I> &labels, MutableFst<Arc> *ofst) {
  typedef typename Arc::StateId StateId;
  typedef typename Arc::Weight Weight;

  ofst->DeleteStates();
  StateId cur_state = ofst->AddState();
  ofst->SetStart(cur_state);
  for (size_t i = 0; i < labels.size(); i++) {
    StateId next_state = ofst->AddState();
    Arc arc(labels[i], labels[i], Weight::One(), next_state);
    ofst->AddArc(cur_state, arc);
    cur_state = next_state;
  }
  ofst->SetFinal(cur_state, Weight::One());
}

template <class Arc>
VectorFst<Arc> *MakeLoopFst(const std::vector<const ExpandedFst<Arc> *> &fsts) {
  typedef typename Arc::Weight Weight;
  typedef typename Arc::StateId StateId;
  typedef typename Arc::Label Label;

  VectorFst<Arc> *ans = new VectorFst<Arc>;
  StateId loop_state = ans->AddState();  // = 0.
  ans->SetStart(loop_state);
  ans->SetFinal(loop_state, Weight::One());

  // "cache" is used as an optimization when some of the pointers in "fsts"
  // may have the same value.
  unordered_map<const ExpandedFst<Arc> *, Arc> cache;

  for (Label i = 0; i < static_cast<Label>(fsts.size()); i++) {
    const ExpandedFst<Arc> *fst = fsts[i];
    if (fst == nullptr) continue;
    {  // optimization with cache: helpful if some members of "fsts" may
      // contain the same pointer value (e.g. in GetHTransducer).
      auto iter = cache.find(fst);
      if (iter != cache.end()) {
        Arc arc = iter->second;
        arc.olabel = i;
        ans->AddArc(0, arc);
        continue;
      }
    }

    KALDIFST_ASSERT(fst->Properties(kAcceptor, true) ==
                    kAcceptor);  // expect acceptor.

    StateId fst_num_states = fst->NumStates();
    StateId fst_start_state = fst->Start();

    if (fst_start_state == kNoStateId) continue;  // empty fst.

    bool share_start_state =
        fst->Properties(kInitialAcyclic, true) == kInitialAcyclic &&
        fst->NumArcs(fst_start_state) == 1 &&
        fst->Final(fst_start_state) == Weight::Zero();

    std::vector<StateId> state_map(fst_num_states);  // fst state -> ans state
    for (StateId s = 0; s < fst_num_states; s++) {
      if (s == fst_start_state && share_start_state)
        state_map[s] = loop_state;
      else
        state_map[s] = ans->AddState();
    }
    if (!share_start_state) {
      Arc arc(0, i, Weight::One(), state_map[fst_start_state]);
      cache[fst] = arc;
      ans->AddArc(0, arc);
    }
    for (StateId s = 0; s < fst_num_states; s++) {
      // Add arcs out of state s.
      for (ArcIterator<ExpandedFst<Arc>> aiter(*fst, s); !aiter.Done();
           aiter.Next()) {
        const Arc &arc = aiter.Value();
        Label olabel = (s == fst_start_state && share_start_state ? i : 0);
        Arc newarc(arc.ilabel, olabel, arc.weight, state_map[arc.nextstate]);
        ans->AddArc(state_map[s], newarc);
        if (s == fst_start_state && share_start_state) cache[fst] = newarc;
      }
      if (fst->Final(s) != Weight::Zero()) {
        KALDIFST_ASSERT(!(s == fst_start_state && share_start_state));
        ans->AddArc(state_map[s], Arc(0, 0, fst->Final(s), loop_state));
      }
    }
  }
  return ans;
}

template <class Arc>
void ApplyProbabilityScale(float scale, MutableFst<Arc> *fst) {
  typedef typename Arc::Weight Weight;
  typedef typename Arc::StateId StateId;
  for (StateIterator<MutableFst<Arc>> siter(*fst); !siter.Done();
       siter.Next()) {
    StateId s = siter.Value();
    for (MutableArcIterator<MutableFst<Arc>> aiter(fst, s); !aiter.Done();
         aiter.Next()) {
      Arc arc = aiter.Value();
      arc.weight = Weight(arc.weight.Value() * scale);
      aiter.SetValue(arc);
    }
    if (fst->Final(s) != Weight::Zero())
      fst->SetFinal(s, Weight(fst->Final(s).Value() * scale));
  }
}

template <class Arc, class I>
class RemoveSomeInputSymbolsMapper {
 public:
  Arc operator()(const Arc &arc_in) {
    Arc ans = arc_in;
    if (to_remove_set_.count(ans.ilabel) != 0)
      ans.ilabel = 0;  // remove this symbol
    return ans;
  }
  MapFinalAction FinalAction() { return MAP_NO_SUPERFINAL; }
  MapSymbolsAction InputSymbolsAction() { return MAP_CLEAR_SYMBOLS; }
  MapSymbolsAction OutputSymbolsAction() { return MAP_COPY_SYMBOLS; }
  uint64_t Properties(uint64_t props) const {
    // remove the following as we don't know now if any of them are true.
    uint64_t to_remove = kAcceptor | kNotAcceptor | kIDeterministic |
                         kNonIDeterministic | kNoEpsilons | kNoIEpsilons |
                         kILabelSorted | kNotILabelSorted;
    return props & ~to_remove;
  }
  RemoveSomeInputSymbolsMapper(const std::vector<I> &to_remove)
      : to_remove_set_(to_remove) {
    static_assert(std::is_integral<I>::value, "");
    assert(to_remove_set_.count(0) == 0);  // makes no sense to remove epsilon.
  }

 private:
  kaldifst::ConstIntegerSet<I> to_remove_set_;
};

template <class Arc, class I>
void RemoveSomeInputSymbols(const std::vector<I> &to_remove,
                            MutableFst<Arc> *fst) {
  static_assert(std::is_integral<I>::value, "");
  RemoveSomeInputSymbolsMapper<Arc, I> mapper(to_remove);
  Map(fst, mapper);
}

}  // namespace fst
#endif  // KALDIFST_CSRC_FSTEXT_UTILS_INL_H_
