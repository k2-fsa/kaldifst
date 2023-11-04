// kaldifst/csrc/text-normalizer.h
//
// Copyright (c)  2023 Xiaomi Corporation

#ifndef KALDIFST_CSRC_TEXT_NORMALIZER_H_
#define KALDIFST_CSRC_TEXT_NORMALIZER_H_

#include <memory>
#include <string>

#include "fst/fst.h"
#include "fst/fstlib.h"

namespace kaldifst {

class TextNormalizer {
 public:
  // Path to rule.fst
  explicit TextNormalizer(const std::string &rule);

  explicit TextNormalizer(std::unique_ptr<fst::StdConstFst> rule);

  // @param s The input text to be normalized
  // @param remove_output_zero True to remove bytes whose value is 0 from the
  //                           output.
  std::string Normalize(const std::string &s,
                        bool remove_output_zero = true) const;

 private:
  std::unique_ptr<fst::StdConstFst> rule_;
};

}  // namespace kaldifst

#endif  // KALDIFST_CSRC_TEXT_NORMALIZER_H_
