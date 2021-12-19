// kaldifst/csrc/kaldi-fst-io-test.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/csrc/kaldi-fst-io.h"

#include <stdio.h>

#include "gtest/gtest.h"

namespace kaldifst {

static fst::StdVectorFst BuildExampleFst() {
  fst::StdVectorFst ans;
  auto s0 = ans.AddState();
  ans.SetStart(s0);
  int32_t ilable = 1;
  int32_t olable = ilable;
  float weight = 0.5;
  auto s1 = ans.AddState();
  ans.AddArc(s0, fst::StdArc(ilable, olable, weight, s1));
  float final_weight = 1.5;
  ans.SetFinal(s1, final_weight);
  return ans;
}

TEST(TestKaldiFstIO, ReadWrite) {
  auto fst1 = BuildExampleFst();
  std::string filename = "a.fst";
  fst::WriteFstKaldi(fst1, filename);

  fst::StdVectorFst fst2;
  fst::ReadFstKaldi(filename, &fst2);
  ASSERT_TRUE(fst::Equivalent(fst1, fst2));

  remove(filename.c_str());
}

}  // namespace kaldifst
