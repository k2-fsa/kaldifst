// kaldifst/csrc/kaldi-holder-inl.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/kaldi-holder-inl.h

// Copyright 2009-2011     Microsoft Corporation
//                2016     Johns Hopkins University (author: Daniel Povey)
//                2016     Xiaohui Zhang
//
#ifndef KALDIFST_CSRC_KALDI_HOLDER_INL_H_
#define KALDIFST_CSRC_KALDI_HOLDER_INL_H_

#include <iostream>
#include <string>

#include "kaldifst/csrc/log.h"
#include "kaldifst/csrc/text-utils.h"

namespace kaldifst {

// We define a Token as a nonempty, printable, whitespace-free std::string.
// The binary and text formats here are the same (newline-terminated)
// and as such we don't bother with the binary-mode headers.
class TokenHolder {
 public:
  typedef std::string T;

  TokenHolder() {}

  static bool Write(std::ostream &os, bool, const T &t) {  // ignore binary-mode
    KALDIFST_ASSERT(IsToken(t));
    os << t << '\n';
    return os.good();
  }

  void Clear() { t_.clear(); }

  // Reads into the holder.
  bool Read(std::istream &is) {
    is >> t_;
    if (is.fail()) return false;
    char c;
    while (isspace(c = is.peek()) && c != '\n') is.get();
    if (is.peek() != '\n') {
      KALDIFST_WARN << "TokenHolder::Read, expected newline, got char "
                    << CharToString(is.peek()) << ", at stream pos "
                    << is.tellg();
      return false;
    }
    is.get();  // get '\n'
    return true;
  }

  // Since this is fundamentally a text format, read in text mode (would work
  // fine either way, but doing it this way will exercise more of the code).
  static bool IsReadInBinary() { return false; }

  T &Value() { return t_; }

  ~TokenHolder() {}

  void Swap(TokenHolder *other) { t_.swap(other->t_); }

  bool ExtractRange(const TokenHolder &other, const std::string &range) {
    KALDIFST_ERR << "ExtractRange is not defined for this type of holder.";
    return false;
  }

 private:
  KALDIFST_DISALLOW_COPY_AND_ASSIGN(TokenHolder);
  T t_;
};

}  // namespace kaldifst

#endif  // KALDIFST_CSRC_KALDI_HOLDER_INL_H_
