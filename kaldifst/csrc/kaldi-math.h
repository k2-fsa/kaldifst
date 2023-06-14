// kaldifst/csrc/kaldi-math.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/base/kaldi-math.h
//
// Copyright 2009-2011  Ondrej Glembek;  Microsoft Corporation;  Yanmin Qian;
//                      Jan Silovsky;  Saarland University
//
#ifndef KALDIFST_CSRC_KALDI_MATH_H_
#define KALDIFST_CSRC_KALDI_MATH_H_

#include <cstdint>

namespace kaldifst {

struct RandomState {
  RandomState();
  unsigned seed;
};

int32_t Rand(struct RandomState *state = nullptr);

// Returns a random integer between first and last inclusive.
int32_t RandInt(int32_t first, int32_t last,
                struct RandomState *state = nullptr);

}  // namespace kaldifst

#endif  // KALDIFST_CSRC_KALDI_MATH_H_
