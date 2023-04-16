// kaldifst/csrc/const-integer-set-inl.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/const-integer-set-inl.h
//
// Copyright 2009-2011     Microsoft Corporation

#ifndef KALDIFST_CSRC_CONST_INTEGER_SET_INL_H_
#define KALDIFST_CSRC_CONST_INTEGER_SET_INL_H_
#include <type_traits>

// Do not include this file directly.  It is included by const-integer-set.h

namespace kaldifst {

template <class I>
void ConstIntegerSet<I>::InitInternal() {
  static_assert(std::is_integral<I>::value, "");
  quick_set_.clear();  // just in case we previously had data.
  if (slow_set_.size() == 0) {
    lowest_member_ = (I)1;
    highest_member_ = (I)0;
    contiguous_ = false;
    quick_ = false;
  } else {
    lowest_member_ = slow_set_.front();
    highest_member_ = slow_set_.back();
    size_t range = highest_member_ + 1 - lowest_member_;
    if (range == slow_set_.size()) {
      contiguous_ = true;
      quick_ = false;
    } else {
      contiguous_ = false;
      // If it would be more compact to store as bool
      if (range < slow_set_.size() * 8 * sizeof(I)) {
        // (assuming 1 bit per element)...
        quick_set_.resize(range, false);
        for (size_t i = 0; i < slow_set_.size(); i++)
          quick_set_[slow_set_[i] - lowest_member_] = true;
        quick_ = true;
      } else {
        quick_ = false;
      }
    }
  }
}

template <class I>
int ConstIntegerSet<I>::count(I i) const {
  if (i < lowest_member_ || i > highest_member_) {
    return 0;
  } else {
    if (contiguous_) return true;
    if (quick_) {
      return (quick_set_[i - lowest_member_] ? 1 : 0);
    } else {
      bool ans = std::binary_search(slow_set_.begin(), slow_set_.end(), i);
      return (ans ? 1 : 0);
    }
  }
}

template <class I>
void ConstIntegerSet<I>::Write(std::ostream &os, bool binary) const {
  WriteIntegerVector(os, binary, slow_set_);
}

template <class I>
void ConstIntegerSet<I>::Read(std::istream &is, bool binary) {
  ReadIntegerVector(is, binary, &slow_set_);
  InitInternal();
}

}  // namespace kaldifst

#endif  // KALDIFST_CSRC_CONST_INTEGER_SET_INL_H_
