// kaldifst/csrc/grammar-context-fst.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/grammar-context-fst.h

// Copyright 2009-2011  Microsoft Corporation

#ifndef KALDIFST_CSRC_GRAMMAR_CONTEXT_FST_H_
#define KALDIFST_CSRC_GRAMMAR_CONTEXT_FST_H_

#include "fst/fst-decl.h"
#include "fst/fstlib.h"

namespace fst {

enum NonterminalValues {
  kNontermBos = 0,      // #nonterm_bos
  kNontermBegin = 1,    // #nonterm_begin
  kNontermEnd = 2,      // #nonterm_end
  kNontermReenter = 3,  // #nonterm_reenter
  kNontermUserDefined =
      4,  // the lowest-numbered user-defined nonterminal, e.g. #nonterm:foo
  // kNontermMediumNumber and kNontermBigNumber come into the encoding of
  // nonterminal-related symbols in HCLG.fst.  The only hard constraint on them
  // is that kNontermBigNumber must be bigger than the biggest transition-id in
  // your system, and kNontermMediumNumber must be >0.  These values were chosen
  // for ease of human inspection of numbers encoded with them.
  kNontermMediumNumber = 1000,
  kNontermBigNumber = 10000000
};

// Returns the smallest multiple of 1000 that is strictly greater than
// nonterm_phones_offset.  Used in the encoding of special symbol in HCLG;
// they are encoded as
//  special_symbol =
//     kNontermBigNumber + (nonterminal * encoding_multiple) + phone_index
inline int32_t GetEncodingMultiple(int32_t nonterm_phones_offset) {
  int32_t medium_number = static_cast<int32_t>(kNontermMediumNumber);
  return medium_number *
         ((nonterm_phones_offset + medium_number) / medium_number);
}

}  // namespace fst

#endif  // KALDIFST_CSRC_GRAMMAR_CONTEXT_FST_H_
