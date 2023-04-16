// kaldifst/csrc/const-integer-set.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/const-integer-set.h

// Copyright 2009-2011  Microsoft Corporation;  Saarland University

#ifndef KALDIFST_CSRC_CONST_INTEGER_SET_H_
#define KALDIFST_CSRC_CONST_INTEGER_SET_H_
#include <algorithm>
#include <cassert>
#include <limits>
#include <set>
#include <vector>

#include "kaldifst/csrc/stl-utils.h"

/* ConstIntegerSet is a way to efficiently test whether something is in a
   supplied set of integers.  It can be initialized from a vector or set, but
   never changed after that. It either uses a sorted vector or an array of
   bool, depending on the input.  It behaves like a const version of an STL set,
   with only a subset of the functionality, except all the member functions are
   upper-case.

   Note that we could get rid of the member slow_set_, but we'd have to
   do more work to implement an iterator type.  This would save memory.
*/

namespace kaldifst {

template <class I>
class ConstIntegerSet {
 public:
  ConstIntegerSet() : lowest_member_(1), highest_member_(0) {}

  void Init(const std::vector<I> &input) {
    slow_set_ = input;
    SortAndUniq(&slow_set_);
    InitInternal();
  }

  void Init(const std::set<I> &input) {
    CopySetToVector(input, &slow_set_);
    InitInternal();
  }

  explicit ConstIntegerSet(const std::vector<I> &input) : slow_set_(input) {
    SortAndUniq(&slow_set_);
    InitInternal();
  }
  explicit ConstIntegerSet(const std::set<I> &input) {
    CopySetToVector(input, &slow_set_);
    InitInternal();
  }
  explicit ConstIntegerSet(const ConstIntegerSet<I> &other)
      : slow_set_(other.slow_set_) {
    InitInternal();
  }

  int count(I i) const;  // returns 1 or 0.

  typedef typename std::vector<I>::const_iterator iterator;
  iterator begin() const { return slow_set_.begin(); }
  iterator end() const { return slow_set_.end(); }
  size_t size() const { return slow_set_.size(); }
  bool empty() const { return slow_set_.empty(); }

  void Write(std::ostream &os, bool binary) const;
  void Read(std::istream &is, bool binary);

 private:
  I lowest_member_;
  I highest_member_;
  bool contiguous_;
  bool quick_;
  std::vector<bool> quick_set_;
  std::vector<I> slow_set_;
  void InitInternal();
};

}  // namespace kaldifst

#include "kaldifst/csrc/const-integer-set-inl.h"

#endif  // KALDIFST_CSRC_CONST_INTEGER_SET_H_
