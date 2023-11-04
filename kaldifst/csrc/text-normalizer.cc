// kaldifst/csrc/text-normalizer.cc
//
// Copyright (c)  2023 Xiaomi Corporation

#include "kaldifst/csrc/text-normalizer.h"

#include <memory>
#include <string>
#include <utility>

#include "fst/arcsort.h"
#include "kaldifst/csrc/kaldi-fst-io.h"
#include "kaldifst/csrc/table-matcher.h"

namespace fst {

constexpr uint64_t kCompiledStringProps =
    kAcceptor | kIDeterministic | kODeterministic | kILabelSorted |
    kOLabelSorted | kUnweighted | kAcyclic | kInitialAcyclic | kTopSorted |
    kAccessible | kCoAccessible | kString | kUnweightedCycles;
}

namespace kaldifst {

// We don't use StringCompiler<StdArc> here since it treats bytes as
// signed integers.
static fst::StdVectorFst StringToFst(const std::string &text) {
  using Weight = typename fst::StdArc::Weight;
  using Arc = fst::StdArc;

  fst::StdVectorFst ans;
  ans.ReserveStates(text.size());

  auto s = ans.AddState();
  ans.SetStart(s);
  // CAUTION(fangjun): We need to use uint8_t here.
  for (const uint8_t label : text) {
    const auto nextstate = ans.AddState();
    ans.AddArc(s, Arc(label, label, Weight::One(), nextstate));
    s = nextstate;
  }

  ans.SetFinal(s, Weight::One());
  ans.SetProperties(fst::kCompiledStringProps, fst::kCompiledStringProps);

  return ans;
}

TextNormalizer::TextNormalizer(const std::string &rule) {
  rule_ = std::unique_ptr<fst::StdConstFst>(
      CastOrConvertToConstFst(fst::ReadFstKaldiGeneric(rule)));
}

TextNormalizer::TextNormalizer(std::unique_ptr<fst::StdConstFst> rule)
    : rule_(std::move(rule)) {}

std::string TextNormalizer::Normalize(const std::string &s) const {
  // Step 1: Convert the input text into an FST
  fst::StdVectorFst text = StringToFst(s);

  // Step 2: Compose the input text with the rule FST
  fst::StdVectorFst composed_fst;
  fst::Compose(text, *rule_, &composed_fst);

  // Step 3: Get the best path from the composed FST
  fst::StdVectorFst one_best;
  fst::ShortestPath(composed_fst, &one_best, 1);

  // Step 4: Concatenate the output labels of the best path
  fst::StringPrinter<fst::StdArc> string_printer(fst::StringTokenType::BYTE);

  std::string normalized;
  string_printer(one_best, &normalized);

  return normalized;
}

}  // namespace kaldifst
