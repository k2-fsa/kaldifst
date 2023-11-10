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

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "kaldifst/csrc/const-integer-set.h"
#include "kaldifst/csrc/determinize-star.h"
#include "kaldifst/csrc/kaldi-math.h"
#include "kaldifst/csrc/stl-utils.h"

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
  explicit RemoveSomeInputSymbolsMapper(const std::vector<I> &to_remove)
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

template <class T>
struct IdentityFunction {
  typedef T Arg;
  typedef T Result;
  T operator()(const T &t) const { return t; }
};

template <class Arc, class F>
void MakePrecedingInputSymbolsSameClass(bool start_is_epsilon,
                                        MutableFst<Arc> *fst, const F &f) {
  typedef typename F::Result ClassType;
  typedef typename Arc::StateId StateId;
  typedef typename Arc::Weight Weight;
  std::vector<ClassType> classes;
  ClassType noClass = f(kNoLabel);
  ClassType epsClass = f(0);
  if (start_is_epsilon) {  // treat having-start-state as epsilon in-transition.
    StateId start_state = fst->Start();
    if (start_state < 0 || start_state == kNoStateId)  // empty FST.
      return;
    classes.resize(start_state + 1, noClass);
    classes[start_state] = epsClass;
  }

  // Find bad states (states with multiple input-symbols into them).
  std::set<StateId> bad_states;  // states that we need to change.
  for (StateIterator<Fst<Arc>> siter(*fst); !siter.Done(); siter.Next()) {
    StateId s = siter.Value();
    for (ArcIterator<Fst<Arc>> aiter(*fst, s); !aiter.Done(); aiter.Next()) {
      const Arc &arc = aiter.Value();
      if (classes.size() <= static_cast<size_t>(arc.nextstate))
        classes.resize(arc.nextstate + 1, noClass);
      if (classes[arc.nextstate] == noClass)
        classes[arc.nextstate] = f(arc.ilabel);
      else if (classes[arc.nextstate] != f(arc.ilabel))
        bad_states.insert(arc.nextstate);
    }
  }
  if (bad_states.empty()) return;  // Nothing to do.
  kaldifst::ConstIntegerSet<StateId> bad_states_ciset(
      bad_states);  // faster lookup.

  // Work out list of arcs we have to change as (state, arc-offset).
  // Can't do the actual changes in this pass, since we have to add new
  // states which invalidates the iterators.
  std::vector<std::pair<StateId, size_t>> arcs_to_change;
  for (StateIterator<Fst<Arc>> siter(*fst); !siter.Done(); siter.Next()) {
    StateId s = siter.Value();
    for (ArcIterator<Fst<Arc>> aiter(*fst, s); !aiter.Done(); aiter.Next()) {
      const Arc &arc = aiter.Value();
      if (arc.ilabel != 0 && bad_states_ciset.count(arc.nextstate) != 0)
        arcs_to_change.push_back(std::make_pair(s, aiter.Position()));
    }
  }
  KALDIFST_ASSERT(!arcs_to_change.empty());  // since !bad_states.empty().

  std::map<std::pair<StateId, ClassType>, StateId> state_map;
  // state_map is a map from (bad-state, input-symbol-class) to dummy-state.

  for (size_t i = 0; i < arcs_to_change.size(); i++) {
    StateId s = arcs_to_change[i].first;
    ArcIterator<MutableFst<Arc>> aiter(*fst, s);
    aiter.Seek(arcs_to_change[i].second);
    Arc arc = aiter.Value();

    // Transition is non-eps transition to "bad" state.  Introduce new state (or
    // find existing one).
    std::pair<StateId, ClassType> p(arc.nextstate, f(arc.ilabel));
    if (state_map.count(p) == 0) {
      StateId newstate = state_map[p] = fst->AddState();
      fst->AddArc(newstate, Arc(0, 0, Weight::One(), arc.nextstate));
    }
    StateId dst_state = state_map[p];
    arc.nextstate = dst_state;

    // Initialize the MutableArcIterator only now, as the call to NewState()
    // may have invalidated the first arc iterator.
    MutableArcIterator<MutableFst<Arc>> maiter(fst, s);
    maiter.Seek(arcs_to_change[i].second);
    maiter.SetValue(arc);
  }
}

template <class Arc>
void MakeFollowingInputSymbolsSame(bool end_is_epsilon, MutableFst<Arc> *fst) {
  IdentityFunction<typename Arc::Label> f;
  MakeFollowingInputSymbolsSameClass(end_is_epsilon, fst, f);
}

template <class Arc, class F>
void MakeFollowingInputSymbolsSameClass(bool end_is_epsilon,
                                        MutableFst<Arc> *fst, const F &f) {
  typedef typename Arc::StateId StateId;
  typedef typename Arc::Weight Weight;
  typedef typename F::Result ClassType;
  std::vector<StateId> bad_states;
  ClassType noClass = f(kNoLabel);
  ClassType epsClass = f(0);
  for (StateIterator<Fst<Arc>> siter(*fst); !siter.Done(); siter.Next()) {
    StateId s = siter.Value();
    ClassType c = noClass;
    bool bad = false;
    for (ArcIterator<Fst<Arc>> aiter(*fst, s); !aiter.Done(); aiter.Next()) {
      const Arc &arc = aiter.Value();
      if (c == noClass) {
        c = f(arc.ilabel);
      } else if (c != f(arc.ilabel)) {
        bad = true;
        break;
      }
    }
    if (end_is_epsilon && c != noClass && c != epsClass &&
        fst->Final(s) != Weight::Zero())
      bad = true;
    if (bad) bad_states.push_back(s);
  }
  std::vector<Arc> my_arcs;
  for (size_t i = 0; i < bad_states.size(); i++) {
    StateId s = bad_states[i];
    my_arcs.clear();
    for (ArcIterator<MutableFst<Arc>> aiter(*fst, s); !aiter.Done();
         aiter.Next())
      my_arcs.push_back(aiter.Value());

    for (size_t j = 0; j < my_arcs.size(); j++) {
      Arc &arc = my_arcs[j];
      if (arc.ilabel != 0) {
        StateId newstate = fst->AddState();
        // Create a new state for each non-eps arc in original FST, out of each
        // bad state. Not as optimal as it could be, but does avoid some
        // complicated weight-pushing issues in which, to maintain
        // stochasticity, we would have to know which semiring we want to
        // maintain stochasticity in.
        fst->AddArc(newstate, Arc(arc.ilabel, 0, Weight::One(), arc.nextstate));
        MutableArcIterator<MutableFst<Arc>> maiter(fst, s);
        maiter.Seek(j);
        maiter.SetValue(Arc(0, arc.olabel, arc.weight, newstate));
      }
    }
  }
}

// return arc-offset of self-loop with ilabel (or -1 if none exists).
// if more than one such self-loop, pick first one.
template <class Arc>
ssize_t FindSelfLoopWithILabel(const Fst<Arc> &fst, typename Arc::StateId s) {
  for (ArcIterator<Fst<Arc>> aiter(fst, s); !aiter.Done(); aiter.Next())
    if (aiter.Value().nextstate == s && aiter.Value().ilabel != 0)
      return static_cast<ssize_t>(aiter.Position());
  return static_cast<ssize_t>(-1);
}

template <class Arc>
bool EqualAlign(const Fst<Arc> &ifst, typename Arc::StateId length,
                int rand_seed, MutableFst<Arc> *ofst, int num_retries) {
  srand(rand_seed);
  KALDIFST_ASSERT(ofst->NumStates() == 0);  // make sure ofst empty.
  // make sure all states can reach final-state (or this algorithm may enter
  // infinite loop.
  KALDIFST_ASSERT(ifst.Properties(kCoAccessible, true) == kCoAccessible);

  typedef typename Arc::StateId StateId;
  typedef typename Arc::Weight Weight;

  if (ifst.Start() == kNoStateId) {
    KALDIFST_WARN << "Empty input fst.";
    return false;
  }
  // First select path through ifst.
  std::vector<StateId> path;
  std::vector<size_t> arc_offsets;  // arc taken out of each state.
  std::vector<int> nof_ilabels;

  StateId num_ilabels = 0;
  int retry_no = 0;

  // Under normal circumstances, this will be one-pass-only process
  // Multiple tries might be needed in special cases, typically when
  // the number of frames is close to number of transitions from
  // the start node to the final node. It usually happens for really
  // short utterances
  do {
    num_ilabels = 0;
    arc_offsets.clear();
    path.clear();
    path.push_back(ifst.Start());

    while (1) {
      // Select either an arc or final-prob.
      StateId s = path.back();
      size_t num_arcs = ifst.NumArcs(s);
      size_t num_arcs_tot = num_arcs;
      if (ifst.Final(s) != Weight::Zero()) num_arcs_tot++;
      // kaldifst::RandInt is a bit like Rand(), but gets around situations
      // where RAND_MAX is very small.
      // Change this to Rand() % num_arcs_tot if compile issues arise
      size_t arc_offset =
          static_cast<size_t>(kaldifst::RandInt(0, num_arcs_tot - 1));

      if (arc_offset < num_arcs) {  // an actual arc.
        ArcIterator<Fst<Arc>> aiter(ifst, s);
        aiter.Seek(arc_offset);
        const Arc &arc = aiter.Value();
        if (arc.nextstate == s) {
          continue;  // don't take this self-loop arc
        } else {
          arc_offsets.push_back(arc_offset);
          path.push_back(arc.nextstate);
          if (arc.ilabel != 0) num_ilabels++;
        }
      } else {
        break;  // Chose final-prob.
      }
    }

    nof_ilabels.push_back(num_ilabels);
  } while ((++retry_no < num_retries) && (num_ilabels > length));

  if (num_ilabels > length) {
    std::stringstream ilabel_vec;
    std::copy(nof_ilabels.begin(), nof_ilabels.end(),
              std::ostream_iterator<int>(ilabel_vec, ","));
    std::string s = ilabel_vec.str();
    s.erase(s.end() - 1);
    KALDIFST_WARN << "EqualAlign: the randomly constructed paths lengths: "
                  << s;
    KALDIFST_WARN << "EqualAlign: utterance has too few frames " << length
                  << " to align.";
    return false;  // can't make it shorter by adding self-loops!.
  }

  StateId num_self_loops = 0;
  std::vector<ssize_t> self_loop_offsets(path.size());
  for (size_t i = 0; i < path.size(); i++)
    if ((self_loop_offsets[i] = FindSelfLoopWithILabel(ifst, path[i])) !=
        static_cast<ssize_t>(-1))
      num_self_loops++;

  if (num_self_loops == 0 && num_ilabels < length) {
    KALDIFST_WARN << "No self-loops on chosen path; cannot match length.";
    return false;  // no self-loops to make it longer.
  }

  StateId num_extra = length - num_ilabels;  // Number of self-loops we need.

  StateId min_num_loops = 0;
  if (num_extra != 0)
    min_num_loops = num_extra / num_self_loops;  // prevent div by zero.
  StateId num_with_one_more_loop = num_extra - (min_num_loops * num_self_loops);
  KALDIFST_ASSERT(num_with_one_more_loop < num_self_loops ||
                  num_self_loops == 0);

  ofst->AddState();
  ofst->SetStart(0);
  StateId cur_state = 0;
  StateId counter = 0;  // tell us when we should stop adding one more loop.
  for (size_t i = 0; i < path.size(); i++) {
    // First, add any self-loops that are necessary.
    StateId num_loops = 0;
    if (self_loop_offsets[i] != static_cast<ssize_t>(-1)) {
      num_loops = min_num_loops + (counter < num_with_one_more_loop ? 1 : 0);
      counter++;
    }
    for (StateId j = 0; j < num_loops; j++) {
      ArcIterator<Fst<Arc>> aiter(ifst, path[i]);
      aiter.Seek(self_loop_offsets[i]);
      Arc arc = aiter.Value();
      KALDIFST_ASSERT(arc.nextstate == path[i] &&
                      arc.ilabel != 0);  // make sure self-loop with ilabel.
      StateId next_state = ofst->AddState();
      ofst->AddArc(cur_state,
                   Arc(arc.ilabel, arc.olabel, arc.weight, next_state));
      cur_state = next_state;
    }
    if (i + 1 < path.size()) {  // add forward transition.
      ArcIterator<Fst<Arc>> aiter(ifst, path[i]);
      aiter.Seek(arc_offsets[i]);
      Arc arc = aiter.Value();
      KALDIFST_ASSERT(arc.nextstate == path[i + 1]);
      StateId next_state = ofst->AddState();
      ofst->AddArc(cur_state,
                   Arc(arc.ilabel, arc.olabel, arc.weight, next_state));
      cur_state = next_state;
    } else {  // add final-prob.
      Weight weight = ifst.Final(path[i]);
      KALDIFST_ASSERT(weight != Weight::Zero());
      ofst->SetFinal(cur_state, weight);
    }
  }
  return true;
}

template <class Arc, class I>
bool GetLinearSymbolSequence(const Fst<Arc> &fst, std::vector<I> *isymbols_out,
                             std::vector<I> *osymbols_out,
                             typename Arc::Weight *tot_weight_out) {
  typedef typename Arc::StateId StateId;
  typedef typename Arc::Weight Weight;

  Weight tot_weight = Weight::One();
  std::vector<I> ilabel_seq;
  std::vector<I> olabel_seq;

  StateId cur_state = fst.Start();
  if (cur_state == kNoStateId) {  // empty sequence.
    if (isymbols_out != nullptr) isymbols_out->clear();
    if (osymbols_out != nullptr) osymbols_out->clear();
    if (tot_weight_out != nullptr) *tot_weight_out = Weight::Zero();
    return true;
  }
  while (1) {
    Weight w = fst.Final(cur_state);
    if (w != Weight::Zero()) {  // is final..
      tot_weight = Times(tot_weight, w);
      if (fst.NumArcs(cur_state) != 0) return false;
      if (isymbols_out != nullptr) *isymbols_out = ilabel_seq;
      if (osymbols_out != nullptr) *osymbols_out = olabel_seq;
      if (tot_weight_out != nullptr) *tot_weight_out = tot_weight;
      return true;
    } else {
      if (fst.NumArcs(cur_state) != 1) return false;

      ArcIterator<Fst<Arc>> iter(fst, cur_state);  // get the only arc.
      const Arc &arc = iter.Value();
      tot_weight = Times(tot_weight, arc.weight);
      if (arc.ilabel != 0) ilabel_seq.push_back(arc.ilabel);
      if (arc.olabel != 0) olabel_seq.push_back(arc.olabel);
      cur_state = arc.nextstate;
    }
  }
}

template <class Arc, class I>
void GetInputSymbols(const Fst<Arc> &fst, bool include_eps,
                     std::vector<I> *symbols) {
  static_assert(std::is_integral<I>::value, "");
  std::unordered_set<I> all_syms;
  for (StateIterator<Fst<Arc>> siter(fst); !siter.Done(); siter.Next()) {
    typename Arc::StateId s = siter.Value();
    for (ArcIterator<Fst<Arc>> aiter(fst, s); !aiter.Done(); aiter.Next()) {
      const Arc &arc = aiter.Value();
      all_syms.insert(arc.ilabel);
    }
  }
  // Remove epsilon, if instructed.
  if (!include_eps && all_syms.count(0) != 0) {
    all_syms.erase(0);
  }

  KALDIFST_ASSERT(symbols != NULL);
  kaldifst::CopySetToVector(all_syms, symbols);
  std::sort(symbols->begin(), symbols->end());
}

// see fstext-utils.h for comment.
template <class Arc>
void ConvertNbestToVector(const Fst<Arc> &fst,
                          std::vector<VectorFst<Arc>> *fsts_out) {
  typedef typename Arc::Weight Weight;
  typedef typename Arc::StateId StateId;
  fsts_out->clear();
  StateId start_state = fst.Start();

  if (start_state == kNoStateId) {
    return;  // No output.
  }

  size_t n_arcs = fst.NumArcs(start_state);
  bool start_is_final = (fst.Final(start_state) != Weight::Zero());
  fsts_out->reserve(n_arcs + (start_is_final ? 1 : 0));

  if (start_is_final) {
    fsts_out->resize(fsts_out->size() + 1);
    StateId start_state_out = fsts_out->back().AddState();
    fsts_out->back().SetFinal(start_state_out, fst.Final(start_state));
  }

  for (ArcIterator<Fst<Arc>> start_aiter(fst, start_state); !start_aiter.Done();
       start_aiter.Next()) {
    fsts_out->resize(fsts_out->size() + 1);
    VectorFst<Arc> &ofst = fsts_out->back();
    const Arc &first_arc = start_aiter.Value();
    StateId cur_state = start_state, cur_ostate = ofst.AddState();
    ofst.SetStart(cur_ostate);
    StateId next_ostate = ofst.AddState();
    ofst.AddArc(cur_ostate, Arc(first_arc.ilabel, first_arc.olabel,
                                first_arc.weight, next_ostate));
    cur_state = first_arc.nextstate;
    cur_ostate = next_ostate;
    while (1) {
      size_t this_n_arcs = fst.NumArcs(cur_state);
      KALDIFST_ASSERT(this_n_arcs <= 1);  // or it violates our assumptions
                                          // about the input.
      if (this_n_arcs == 1) {
        KALDIFST_ASSERT(fst.Final(cur_state) == Weight::Zero());
        // or problem with ShortestPath.
        ArcIterator<Fst<Arc>> aiter(fst, cur_state);
        const Arc &arc = aiter.Value();
        next_ostate = ofst.AddState();
        ofst.AddArc(cur_ostate,
                    Arc(arc.ilabel, arc.olabel, arc.weight, next_ostate));
        cur_state = arc.nextstate;
        cur_ostate = next_ostate;
      } else {
        KALDIFST_ASSERT(fst.Final(cur_state) != Weight::Zero());
        // or problem with ShortestPath.
        ofst.SetFinal(cur_ostate, fst.Final(cur_state));
        break;
      }
    }
  }
}

}  // namespace fst
#endif  // KALDIFST_CSRC_FSTEXT_UTILS_INL_H_
