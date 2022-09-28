// kaldifst/python/csrc/fstreverse.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstreverse.h"

#include <memory>
#include <utility>

#include "fst/reverse.h"

namespace kaldifst {

void PybindFstReverse(py::module &m) {  // NOLINT
  m.def(
      "reverse",
      [](const fst::StdFst &ifst, bool require_superinitial = true)
          -> std::unique_ptr<fst::StdVectorFst> {
        auto ans = std::make_unique<fst::StdVectorFst>();
        fst::Reverse(ifst, ans.get(), require_superinitial);
        return std::move(ans);
      },
      py::arg("ifst"), py::arg("require_superinitial") = true);
}

}  // namespace kaldifst
