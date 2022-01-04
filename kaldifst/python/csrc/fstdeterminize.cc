// kaldifst/python/csrc/fstdeterminize.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstdeterminize.h"

#include "fst/determinize.h"
#include "fst/script/determinize.h"
#include "fst/script/getters.h"
#include "kaldifst/csrc/log.h"

namespace kaldifst {

void PybindFstDeterminize(py::module &m) {
  m.def(
      "determinize",
      [](const fst::StdFst &ifst, double delta = fst::kDelta,
         const std::string &det_type = "functional",
         bool increment_subsequential_label = false,
         int64_t nstate = fst::kNoStateId, int64_t subsequential_label = 0,
         const std::string &weight = "") -> std::unique_ptr<fst::StdVectorFst> {
        using fst::script::WeightClass;
        const auto weight_threshold =
            weight.empty() ? WeightClass::Zero(fst::TropicalWeight::Type())
                           : WeightClass(fst::TropicalWeight::Type(), weight);

        fst::DeterminizeType fst_det_type;
        if (!fst::script::GetDeterminizeType(det_type, &fst_det_type)) {
          KALDIFST_ERR << "Unknown or unsupported determinization type: "
                       << det_type;
        }

        fst::DeterminizeOptions<fst::StdArc> opts;
        opts.delta = delta;
        opts.weight_threshold =
            *(weight_threshold.GetWeight<fst::TropicalWeight>());
        opts.state_threshold = nstate;
        opts.subsequential_label = subsequential_label;
        opts.type = fst_det_type;
        opts.increment_subsequential_label = increment_subsequential_label;

        auto ans = std::make_unique<fst::StdVectorFst>();
        fst::Determinize(ifst, ans.get(), opts);
        return std::move(ans);
      },
      py::arg("ifst"), py::arg("delta") = fst::kDelta,
      py::arg("det_type") = "functional",
      py::arg("increment_subsequential_label") = false,
      py::arg("nstate") = fst::kNoStateId, py::arg("subsequential_label") = 0,
      py::arg("weight") = "");
}

}  // namespace kaldifst
