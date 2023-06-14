// kaldifst/csrc/stl-utils.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/stl-utils.h

// Copyright 2009-2011  Microsoft Corporation;  Saarland University

#ifndef KALDIFST_CSRC_STL_UTILS_H_
#define KALDIFST_CSRC_STL_UTILS_H_
#include <algorithm>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_set>
#include <utility>
#include <vector>

#include "kaldifst/csrc/log.h"

namespace kaldifst {

/// A hashing function object for strings.
struct StringHasher {  // hashing function for std::string
  size_t operator()(const std::string &str) const noexcept {
    size_t ans = 0, len = str.length();
    const char *c = str.c_str(), *end = c + len;
    for (; c != end; c++) {
      ans *= kPrime;
      ans += *c;
    }
    return ans;
  }

 private:
  static const int kPrime = 7853;
};

/// A hashing function-object for vectors.
template <typename Int>
struct VectorHasher {  // hashing function for vector<Int>.
  size_t operator()(const std::vector<Int> &x) const noexcept {
    size_t ans = 0;
    typename std::vector<Int>::const_iterator iter = x.begin(), end = x.end();
    for (; iter != end; ++iter) {
      ans *= kPrime;
      ans += *iter;
    }
    return ans;
  }
  VectorHasher() {  // Check we're instantiated with an integer type.
    static_assert(std::is_integral<Int>::value, "");
  }

 private:
  static const int kPrime = 7853;
};

/// Sorts and uniq's (removes duplicates) from a vector.
template <typename T>
inline void SortAndUniq(std::vector<T> *vec) {
  std::sort(vec->begin(), vec->end());
  vec->erase(std::unique(vec->begin(), vec->end()), vec->end());
}

/// A hashing function-object for pairs of ints
template <typename Int1, typename Int2 = Int1>
struct PairHasher {  // hashing function for pair<int>
  size_t operator()(const std::pair<Int1, Int2> &x) const noexcept {
    // 7853 was chosen at random from a list of primes.
    return x.first + x.second * 7853;
  }
  PairHasher() {  // Check we're instantiated with an integer type.
    static_assert(std::is_integral<Int1>::value, "");
    static_assert(std::is_integral<Int2>::value, "");
  }
};

/// Copies the elements of a set to a vector.
template <class T>
void CopySetToVector(const std::set<T> &s, std::vector<T> *v) {
  // copies members of s into v, in sorted order from lowest to highest
  // (because the set was in sorted order).
  KALDIFST_ASSERT(v != nullptr);
  v->resize(s.size());
  auto siter = s.begin(), send = s.end();
  auto viter = v->begin();
  for (; siter != send; ++siter, ++viter) {
    *viter = *siter;
  }
}

template <class T>
void CopySetToVector(const std::unordered_set<T> &s, std::vector<T> *v) {
  KALDIFST_ASSERT(v != nullptr);
  v->resize(s.size());
  auto siter = s.begin(), send = s.end();
  auto viter = v->begin();
  for (; siter != send; ++siter, ++viter) {
    *viter = *siter;
  }
}

}  // namespace kaldifst

#endif  // KALDIFST_CSRC_STL_UTILS_H_
