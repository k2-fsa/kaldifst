// kaldifst/csrc/context-fst.cc
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/context-fst.cc

// Copyright      2018  Johns Hopkins University (author: Daniel Povey)
#include "kaldifst/csrc/context-fst.h"
namespace fst {

void AddSubsequentialLoop(StdArc::Label subseq_symbol,
                          MutableFst<StdArc> *fst) {
  typedef StdArc Arc;
  typedef typename Arc::StateId StateId;
  typedef typename Arc::Weight Weight;

  vector<StateId> final_states;
  for (StateIterator<MutableFst<Arc>> siter(*fst); !siter.Done();
       siter.Next()) {
    StateId s = siter.Value();
    if (fst->Final(s) != Weight::Zero()) final_states.push_back(s);
  }

  StateId superfinal = fst->AddState();
  Arc arc(subseq_symbol, 0, Weight::One(), superfinal);
  fst->AddArc(superfinal, arc);  // loop at superfinal.
  fst->SetFinal(superfinal, Weight::One());

  for (size_t i = 0; i < final_states.size(); i++) {
    StateId s = final_states[i];
    fst->AddArc(s, Arc(subseq_symbol, 0, fst->Final(s), superfinal));
    // No, don't remove the final-weights of the original states..
    // this is so we can add the subsequential loop in cases where
    // there is no context, and it won't hurt.
    // fst->SetFinal(s, Weight::Zero());
    arc.nextstate = final_states[i];  // TODO(fangjun): remove it?
  }
}

}  // namespace fst
