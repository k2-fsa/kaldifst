// kaldifst/python/csrc/fstdeterminize.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstdeterminize.h"

#include "fst/determinize.h"
#include "fst/script/determinize.h"
#include "fst/script/getters.h"
#include "kaldifst/csrc/log.h"

static constexpr const char *kFstDeterminizeDoc = R"doc(
This operation determinizes a weighted transducer.

The result will be an equivalent FST that has the property that no state has
two transitions with the same input label. For this algorithm, epsilon
transitions are treated as regular symbols (cf. RmEpsilon).

See also `<https://www.openfst.org/twiki/bin/view/FST/DeterminizeDoc>`_

Args:
  ifst:
    The input FST.
  delta:
    Comparison/quantization delta.
  det_type:
    Type of determinization: "functional", "nonfunctional", "disambiguate".
  increment_subsequential_label:
    Increment subsequential_label to obtain distinct labels for  subsequential
    arcs at a given state
  nstate:
    State number threshold
  subsequential_label:
    Input label of arc corresponding to residual final output when producing a
    subsequential transducer
  weight:
    Weight threshold

Returns:
  Return the determinized FST.

**Example 1: Determinize a transducer**

.. literalinclude:: ./code/determinize/ex.py
   :language: python
   :linenos:
   :caption: Determinize a transducer.

.. figure:: ./code/determinize/transducer.svg
    :alt: transducer.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer.svg (before determinization)

.. figure:: ./code/determinize/transducer2.svg
    :alt: transducer2.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer2.svg (after determinization)
)doc";

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
      py::arg("weight") = "", kFstDeterminizeDoc);
}

}  // namespace kaldifst
