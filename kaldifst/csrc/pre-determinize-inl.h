// kaldifst/csrc/pre-determinize-inl.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/pre-determinize-inl.h

// Copyright 2009-2011  Microsoft Corporation

#ifndef KALDIFST_CSRC_PRE_DETERMINIZE_INL_H_
#define KALDIFST_CSRC_PRE_DETERMINIZE_INL_H_

#include <assert.h>

#include <algorithm>
#include <set>
#include <vector>

namespace fst {

// see pre-determinize.h for documentation.
template <class Arc>
void AddSelfLoops(MutableFst<Arc> *fst,
                  const std::vector<typename Arc::Label> &isyms,
                  const std::vector<typename Arc::Label> &osyms) {
  assert(fst != nullptr);
  assert(isyms.size() == osyms.size());
  typedef typename Arc::Label Label;
  typedef typename Arc::StateId StateId;
  typedef typename Arc::Weight Weight;
  size_t n = isyms.size();
  if (n == 0) return;  // Nothing to do.

  // {
  // the following declarations and statements are for quick detection of these
  // symbols, which is purely for debugging/checking purposes.
  Label isyms_min = *std::min_element(isyms.begin(), isyms.end()),
        isyms_max = *std::max_element(isyms.begin(), isyms.end()),
        osyms_min = *std::min_element(osyms.begin(), osyms.end()),
        osyms_max = *std::max_element(osyms.begin(), osyms.end());
  std::set<Label> isyms_set, osyms_set;
  for (size_t i = 0; i < isyms.size(); i++) {
    assert(isyms[i] > 0 &&
           osyms[i] > 0);  // should not have epsilon or invalid symbols.
    isyms_set.insert(isyms[i]);
    osyms_set.insert(osyms[i]);
  }
  assert(isyms_set.size() == n && osyms_set.size() == n);
  // } end block.

  for (StateIterator<MutableFst<Arc>> siter(*fst); !siter.Done();
       siter.Next()) {
    StateId state = siter.Value();
    bool this_state_needs_self_loops = (fst->Final(state) != Weight::Zero());
    for (ArcIterator<MutableFst<Arc>> aiter(*fst, state); !aiter.Done();
         aiter.Next()) {
      const Arc &arc = aiter.Value();
      // If one of the following asserts fails, it means that the input FST
      // already had the symbols we are inserting.  This is contrary to the
      // preconditions of this algorithm.
      assert(!(arc.ilabel >= isyms_min && arc.ilabel <= isyms_max &&
               isyms_set.count(arc.ilabel) != 0));
      assert(!(arc.olabel >= osyms_min && arc.olabel <= osyms_max &&
               osyms_set.count(arc.olabel) != 0));
      if (arc.olabel != 0)  // Has non-epsilon output label -> need self loops.
        this_state_needs_self_loops = true;
    }
    if (this_state_needs_self_loops) {
      for (size_t i = 0; i < n; i++) {
        Arc arc;
        arc.ilabel = isyms[i];
        arc.olabel = osyms[i];
        arc.weight = Weight::One();
        arc.nextstate = state;
        fst->AddArc(state, arc);
      }
    }
  }
}

template <class T>
void CopySetToVector(const std::set<T> s, std::vector<T> *v) {
  // adds members of s to v, in sorted order from lowest to highest
  // (because the set was in sorted order).
  assert(v != nullptr);
  v->resize(s.size());
  typename std::set<T>::const_iterator siter = s.begin();
  typename std::vector<T>::iterator viter = v->begin();
  for (; siter != s.end(); ++siter, ++viter) {
    assert(viter != v->end());
    *viter = *siter;
  }
}

}  // namespace fst
#endif  // KALDIFST_CSRC_PRE_DETERMINIZE_INL_H_
