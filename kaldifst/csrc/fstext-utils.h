// kaldifst/csrc/fstext-utils.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/fstext-utils.h

// Copyright 2009-2011  Microsoft Corporation
//           2012-2013  Johns Hopkins University (Author: Daniel Povey)
//                2013  Guoguo Chen
//                2014  Telepoint Global Hosting Service, LLC. (Author: David
//                Snyder)

#ifndef KALDIFST_CSRC_FSTEXT_UTILS_H_
#define KALDIFST_CSRC_FSTEXT_UTILS_H_

#include "fst/fst.h"
#include "fst/map.h"
#include "fst/minimize.h"

namespace fst {

// Minimizes after encoding; applicable to all FSTs.  It is like what you get
// from the Minimize() function, except it will not push the weights, or the
// symbols.  This is better for our recipes, as we avoid ever pushing the
// weights.  However, it will only minimize optimally if your graphs are such
// that the symbols are as far to the left as they can go, and the weights
// in combinable paths are the same... hard to formalize this, but it's
// something that is satisfied by our normal FSTs.
template <class Arc>
void MinimizeEncoded(MutableFst<Arc> *fst, float delta = kDelta) {
  Map(fst, QuantizeMapper<Arc>(delta));
  EncodeMapper<Arc> encoder(kEncodeLabels | kEncodeWeights, ENCODE);
  Encode(fst, &encoder);
  internal::AcceptorMinimize(fst);
  Decode(fst, encoder);
}

inline bool DeterminizeStarInLog(VectorFst<StdArc> *fst, float delta = kDelta,
                                 bool *debug_ptr = NULL, int max_states = -1);

/// Creates unweighted linear acceptor from symbol sequence.
template <class Arc, class I>
void MakeLinearAcceptor(const std::vector<I> &labels, MutableFst<Arc> *ofst);

/// MakeLoopFst creates an FST that has a state that is both initial and
/// final (weight == Weight::One()), and for each non-NULL pointer fsts[i],
/// it has an arc out whose output-symbol is i and which goes to a
/// sub-graph whose input language is equivalent to fsts[i], where the
/// final-state becomes a transition to the loop-state.  Each fst in "fsts"
/// should be an acceptor.  The fst MakeLoopFst returns is output-deterministic,
/// but not output-epsilon free necessarily, and arcs are sorted on output
/// label. Note: if some of the pointers in the input vector "fsts" have the
/// same value, "MakeLoopFst" uses this to speed up the computation.

/// Formally: suppose I is the set of indexes i such that fsts[i] != NULL.
/// Let L[i] be the language that the acceptor fsts[i] accepts.
/// Let the language K be the set of input-output pairs i:l such
/// that i in I and l in L[i].  Then the FST returned by MakeLoopFst
/// accepts the language K*, where * is the Kleene closure (CLOSURE_STAR)
/// of K.

/// We could have implemented this via a combination of "project",
/// "concat", "union" and "closure".  But that FST would have been
/// less well optimized and would have a lot of final-states.

template <class Arc>
VectorFst<Arc> *MakeLoopFst(const std::vector<const ExpandedFst<Arc> *> &fsts);

/// ApplyProbabilityScale is applicable to FSTs in the log or tropical semiring.
/// It multiplies the arc and final weights by "scale" [this is not the Mul
/// operation of the semiring, it's actual multiplication, which is equivalent
/// to taking a power in the semiring].
template <class Arc>
void ApplyProbabilityScale(float scale, MutableFst<Arc> *fst);

/// RemoveSomeInputSymbols removes any symbol that appears in "to_remove", from
/// the input side of the FST, replacing them with epsilon.
template <class Arc, class I>
void RemoveSomeInputSymbols(const std::vector<I> &to_remove,
                            MutableFst<Arc> *fst);

/// As MakePrecedingInputSymbolsSame, but takes a functor object that maps
/// labels to classes.
template <class Arc, class F>
void MakePrecedingInputSymbolsSameClass(bool start_is_epsilon,
                                        MutableFst<Arc> *fst, const F &f);

/// MakeFollowingInputSymbolsSame ensures that all arcs exiting any given fst
/// state have the same input symbol.  It does this by detecting states that
/// have differing input symbols on arcs that exit it, and inserting, for each
/// of the following arcs with non-epsilon input symbol, a new dummy state that
/// has an input-epsilon link from the fst state.  The output symbol and weight
/// stay on the link to the dummy state (in order to keep the FST
/// output-deterministic and stochastic, if it already was). If end_is_epsilon,
/// treat "being a final-state" like having an epsilon output link.
template <class Arc>
void MakeFollowingInputSymbolsSame(bool end_is_epsilon, MutableFst<Arc> *fst);

/// As MakeFollowingInputSymbolsSame, but takes a functor object that maps
/// labels to classes.
template <class Arc, class F>
void MakeFollowingInputSymbolsSameClass(bool end_is_epsilon,
                                        MutableFst<Arc> *fst, const F &f);

/// EqualAlign is similar to RandGen, but it generates a sequence with exactly
/// "length" input symbols.  It returns true on success, false on failure
/// (failure is partly random but should never happen in practice for normal
/// speech models.) It generates a random path through the input FST, finds out
/// which subset of the states it visits along the way have self-loops with
/// inupt symbols on them, and outputs a path with exactly enough self-loops to
/// have the requested number of input symbols. Note that EqualAlign does not
/// use the probabilities on the FST.  It just uses equal probabilities in the
/// first stage of selection (since the output will anyway not be a truly random
/// sample from the FST). The input fst "ifst" must be connected or this may
/// enter an infinite loop.
template <class Arc>
bool EqualAlign(const Fst<Arc> &ifst, typename Arc::StateId length,
                int rand_seed, MutableFst<Arc> *ofst, int num_retries = 10);

/// GetLinearSymbolSequence gets the symbol sequence from a linear FST.
/// If the FST is not just a linear sequence, it returns false.   If it is
/// a linear sequence (including the empty FST), it returns true.  In this
/// case it outputs the symbol
/// sequences as "isymbols_out" and "osymbols_out" (removing epsilons), and
/// the total weight as "tot_weight". The total weight will be Weight::Zero()
/// if the FST is empty.  If any of the output pointers are NULL, it does not
/// create that output.
template <class Arc, class I>
bool GetLinearSymbolSequence(const Fst<Arc> &fst, std::vector<I> *isymbols_out,
                             std::vector<I> *osymbols_out,
                             typename Arc::Weight *tot_weight_out);

/// GetInputSymbols gets the list of symbols on the input of fst
/// (including epsilon, if include_eps == true), as a sorted, unique
/// list.
template <class Arc, class I>
void GetInputSymbols(const Fst<Arc> &fst, bool include_eps,
                     std::vector<I> *symbols);

}  // namespace fst

#include "kaldifst/csrc/fstext-utils-inl.h"

#endif  // KALDIFST_CSRC_FSTEXT_UTILS_H_
