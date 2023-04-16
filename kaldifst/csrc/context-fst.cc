// kaldifst/csrc/context-fst.cc
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/context-fst.cc

// Copyright      2018  Johns Hopkins University (author: Daniel Povey)
#include "kaldifst/csrc/context-fst.h"

#include "kaldifst/csrc/log.h"
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

InverseContextFst::InverseContextFst(Label subsequential_symbol,
                                     const vector<int32_t> &phones,
                                     const vector<int32_t> &disambig_syms,
                                     int32_t context_width,
                                     int32_t central_position)
    : context_width_(context_width),
      central_position_(central_position),
      phone_syms_(phones),
      disambig_syms_(disambig_syms),
      subsequential_symbol_(subsequential_symbol) {
  {  // This block checks the inputs.
    KALDIFST_ASSERT(subsequential_symbol != 0 &&
                    disambig_syms_.count(subsequential_symbol) == 0 &&
                    phone_syms_.count(subsequential_symbol) == 0);
    if (phone_syms_.empty())
      KALDIFST_WARN
          << "Context FST created but there are no phone symbols: probably "
             "input FST was empty.";
    KALDIFST_ASSERT(phone_syms_.count(0) == 0 && disambig_syms_.count(0) == 0 &&
                    central_position_ >= 0 &&
                    central_position_ < context_width_);
    for (size_t i = 0; i < phones.size(); i++) {
      KALDIFST_ASSERT(disambig_syms_.count(phones[i]) == 0);
    }
  }

  // empty vector, will be the ilabel_info vector that corresponds to epsilon,
  // in case our FST needs to output epsilons.
  vector<int32_t> empty_vec;
  Label epsilon_label = FindLabel(empty_vec);

  // epsilon_vec is the phonetic context window we have at the very start of a
  // sequence, meaning "no real phones have been seen yet".
  vector<int32_t> epsilon_vec(context_width_ - 1, 0);
  StateId start_state = FindState(epsilon_vec);

  KALDIFST_ASSERT(epsilon_label == 0 && start_state == 0);

  if (context_width_ > central_position_ + 1 && !disambig_syms_.empty()) {
    // We add a symbol whose sequence representation is [ 0 ], and whose
    // symbol-id is 1.  This is treated as a disambiguation symbol, we call it
    // #-1 in printed form.  It is necessary to ensure that all determinizable
    // LG's will have determinizable CLG's.  The problem it fixes is quite
    // subtle-- it relates to reordering of disambiguation symbols (they appear
    // earlier in CLG than in LG, relative to phones), and the fact that if a
    // disambig symbol appears at the very start of a sequence in CLG, it's not
    // clear exactly where it appeared on the corresponding sequence at the
    // input of LG.
    vector<int32_t> pseudo_eps_vec;
    pseudo_eps_vec.push_back(0);
    pseudo_eps_symbol_ = FindLabel(pseudo_eps_vec);
    KALDIFST_ASSERT(pseudo_eps_symbol_ == 1);
  } else {
    pseudo_eps_symbol_ = 0;  // use actual epsilon.
  }
}

void InverseContextFst::ShiftSequenceLeft(Label label,
                                          std::vector<int32_t> *phone_seq) {
  if (!phone_seq->empty()) {
    phone_seq->erase(phone_seq->begin());
    phone_seq->push_back(label);
  }
}

void InverseContextFst::GetFullPhoneSequence(
    const std::vector<int32_t> &seq, Label label,
    std::vector<int32_t> *full_phone_sequence) {
  int32_t context_width = context_width_;
  full_phone_sequence->reserve(context_width);
  full_phone_sequence->insert(full_phone_sequence->end(), seq.begin(),
                              seq.end());
  full_phone_sequence->push_back(label);
  for (int32_t i = central_position_ + 1; i < context_width; i++) {
    if ((*full_phone_sequence)[i] == subsequential_symbol_) {
      (*full_phone_sequence)[i] = 0;
    }
  }
}

InverseContextFst::Weight InverseContextFst::Final(StateId s) {
  KALDIFST_ASSERT(static_cast<size_t>(s) < state_seqs_.size());

  const vector<int32_t> &phone_context = state_seqs_[s];

  KALDIFST_ASSERT(phone_context.size() == context_width_ - 1);

  bool has_final_prob;

  if (central_position_ < context_width_ - 1) {
    has_final_prob =
        (phone_context[central_position_] == subsequential_symbol_);
    // if phone_context[central_position_] != subsequential_symbol_ then we have
    // pending phones-in-context that we still need to output, so we need to
    // consume more subsequential symbols before we can terminate.
  } else {
    has_final_prob = true;
  }
  return has_final_prob ? Weight::One() : Weight::Zero();
}

bool InverseContextFst::GetArc(StateId s, Label ilabel, Arc *arc) {
  KALDIFST_ASSERT(ilabel != 0 && static_cast<size_t>(s) < state_seqs_.size() &&
                  state_seqs_[s].size() == context_width_ - 1);

  if (IsDisambigSymbol(ilabel)) {
    // A disambiguation-symbol self-loop arc.
    CreateDisambigArc(s, ilabel, arc);
    return true;
  } else if (IsPhoneSymbol(ilabel)) {
    const vector<int32_t> &seq = state_seqs_[s];
    if (!seq.empty() && seq.back() == subsequential_symbol_) {
      return false;  // A real phone is not allowed to follow the subsequential
                     // symbol.
    }

    // next_seq will be 'seq' shifted left by 1, with 'ilabel' appended.
    vector<int32_t> next_seq(seq);
    ShiftSequenceLeft(ilabel, &next_seq);

    // full-seq will be the full context window of size context_width_.
    vector<int32_t> full_seq;
    GetFullPhoneSequence(seq, ilabel, &full_seq);

    StateId next_s = FindState(next_seq);

    CreatePhoneOrEpsArc(s, next_s, ilabel, full_seq, arc);
    return true;
  } else if (ilabel == subsequential_symbol_) {
    const vector<int32_t> &seq = state_seqs_[s];

    if (central_position_ + 1 == context_width_ ||
        seq[central_position_] == subsequential_symbol_) {
      // We already had "enough" subsequential symbols in a row and don't want
      // to accept any more, or we'd be making the subsequential symbol the
      // central phone.
      return false;
    }

    // full-seq will be the full context window of size context_width_.
    vector<int32_t> full_seq;
    GetFullPhoneSequence(seq, ilabel, &full_seq);

    vector<int32_t> next_seq(seq);
    ShiftSequenceLeft(ilabel, &next_seq);
    StateId next_s = FindState(next_seq);

    CreatePhoneOrEpsArc(s, next_s, ilabel, full_seq, arc);
    return true;
  } else {
    KALDIFST_ERR << "ContextFst: CreateArc, invalid ilabel supplied [confusion "
                 << "about phone list or disambig symbols?]: " << ilabel;
  }
  return false;  // won't get here.  suppress compiler error.
}

void InverseContextFst::CreateDisambigArc(StateId s, Label ilabel, Arc *arc) {
  // Creates a self-loop arc corresponding to the disambiguation symbol.
  vector<int32_t>
      label_info;  // This will be a vector containing just [ -olabel ].
  label_info.push_back(
      -ilabel);  // olabel is a disambiguation symbol.  Use its negative
                 // so we can more easily distinguish them from phones.
  Label olabel = FindLabel(label_info);
  arc->ilabel = ilabel;
  arc->olabel = olabel;
  arc->weight = Weight::One();
  arc->nextstate = s;  // self-loop.
}

void InverseContextFst::CreatePhoneOrEpsArc(StateId src, StateId dest,
                                            Label ilabel,
                                            const vector<int32_t> &phone_seq,
                                            Arc *arc) {
  KALDIFST_ASSERT(phone_seq[central_position_] != subsequential_symbol_);

  arc->ilabel = ilabel;
  arc->weight = Weight::One();
  arc->nextstate = dest;
  if (phone_seq[central_position_] == 0) {
    // This can happen at the beginning of the graph.  In this case we don't
    // output a real phone, we createdt an epsilon arc (but sometimes we need to
    // use a special disambiguation symbol instead of epsilon).
    arc->olabel = pseudo_eps_symbol_;
  } else {
    // We have a phone in the central position.
    arc->olabel = FindLabel(phone_seq);
  }
}

StdArc::StateId InverseContextFst::FindState(const vector<int32_t> &seq) {
  // Finds state-id corresponding to this vector of phones.  Inserts it if
  // necessary.
  KALDIFST_ASSERT(static_cast<int32_t>(seq.size()) == context_width_ - 1);
  VectorToStateMap::const_iterator iter = state_map_.find(seq);
  if (iter == state_map_.end()) {  // Not already in map.
    StateId this_state_id = (StateId)state_seqs_.size();
    state_seqs_.push_back(seq);
    state_map_[seq] = this_state_id;
    return this_state_id;
  } else {
    return iter->second;
  }
}

StdArc::Label InverseContextFst::FindLabel(const vector<int32_t> &label_vec) {
  // Finds the ilabel corresponding to this vector (creates a new ilabel if
  // necessary).
  VectorToLabelMap::const_iterator iter = ilabel_map_.find(label_vec);
  if (iter == ilabel_map_.end()) {  // Not already in map.
    Label this_label = ilabel_info_.size();
    ilabel_info_.push_back(label_vec);
    ilabel_map_[label_vec] = this_label;
    return this_label;
  } else {
    return iter->second;
  }
}

}  // namespace fst
