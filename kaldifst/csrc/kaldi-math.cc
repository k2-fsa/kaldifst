// kaldifst/csrc/kaldi-math.cc
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/base/kaldi-math.cc
//
// Copyright 2009-2011  Ondrej Glembek;  Microsoft Corporation;  Yanmin Qian;
//                      Jan Silovsky;  Saarland University
//
#include "kaldifst/csrc/kaldi-math.h"

#include <cstdlib>
#include <mutex>  // NOLINT

#include "kaldifst/csrc/log.h"

namespace kaldifst {

RandomState::RandomState() {
  // we initialize it as Rand() + 27437 instead of just Rand(), because on some
  // systems, e.g. at the very least Mac OSX Yosemite and later, it seems to be
  // the case that rand_r when initialized with rand() will give you the exact
  // same sequence of numbers that rand() will give if you keep calling rand()
  // after that initial call.  This can cause problems with repeated sequences.
  // For example if you initialize two RandomState structs one after the other
  // without calling rand() in between, they would give you the same sequence
  // offset by one (if we didn't have the "+ 27437" in the code).  27437 is just
  // a randomly chosen prime number.
  seed = unsigned(Rand()) + 27437;
}

int32_t Rand(struct RandomState *state /*=nullptr*/) {
#if !defined(_POSIX_THREAD_SAFE_FUNCTIONS)
  // On Windows and Cygwin, just call Rand()
  return rand();
#else
  if (state) {
    return rand_r(&(state->seed));
  } else {
    static std::mutex _RandMutex;
    std::lock_guard<std::mutex> lock(_RandMutex);
    return rand();
  }
#endif
}

int32_t RandInt(int32_t min_val, int32_t max_val,
                struct RandomState *state /*=nullptr*/) {
  // This is not exact.
  KALDIFST_ASSERT(max_val >= min_val);
  if (max_val == min_val) return min_val;

#ifdef _MSC_VER
  // RAND_MAX is quite small on Windows -> may need to handle larger numbers.
  if (RAND_MAX > (max_val - min_val) * 8) {
    // *8 to avoid large inaccuracies in probability, from the modulus...
    return min_val +
           ((unsigned int)Rand(state) % (unsigned int)(max_val + 1 - min_val));
  } else {
    if ((unsigned int)(RAND_MAX * RAND_MAX) >
        (unsigned int)((max_val + 1 - min_val) * 8)) {
      // *8 to avoid inaccuracies in probability, from the modulus...
      return min_val + ((unsigned int)((Rand(state) + RAND_MAX * Rand(state))) %
                        (unsigned int)(max_val + 1 - min_val));
    } else {
      KALDIFST_ERR << "rand_int failed because we do not support such large "
                      "random numbers. (Extend this function).";
    }
  }
#else
  return min_val + (static_cast<int32_t>(Rand(state)) %
                    static_cast<int32_t>(max_val + 1 - min_val));
#endif
}
}  // namespace kaldifst
