// kaldifst/python/csrc/determinize-star.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/csrc/determinize-star.h"

#include "fst/arcsort.h"
#include "kaldifst/csrc/fstext-utils.h"
#include "kaldifst/python/csrc/determinize-star.h"

static constexpr const char *kFstDeterminizeStarDoc = R"doc(
Removes epsilons and determinizes in one step.

See also `<https://github.com/kaldi-asr/kaldi/blob/master/src/fstbin/fstdeterminizestar.cc>`_

.. caution::

    The input FST is modified in-place.

Args:
  in_out:
    The input/output FST. Note it is modified in-place.
  delta:
    Delta value used to determine equivalence of weights.
  max_states:
    Maximum number of states in determinized FST before it will abort.
  use_log:
    Determinize in log semiring.
Returns:
  The return status is un-intuitive: this function will return ``False``
  if determinization completed normally, and true if it was stopped early
  by reaching the 'max-states' limit, and a partial FST was generated.

**Example 1: Determinize a transducer**

.. literalinclude:: ./code/determinizestar/ex.py
   :language: python
   :linenos:
   :caption: Determinize a transducer.

.. figure:: ./code/determinizestar/transducer.svg
    :alt: transducer.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer.svg (before determinization)

.. figure:: ./code/determinizestar/transducer2.svg
    :alt: transducer2.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer2.svg (after determinization)
)doc";

namespace kaldifst {

void PybindDeterminizeStar(py::module &m) {
  m.def(
      "determinize_star",
      [](fst::VectorFst<fst::StdArc> *in_out, float delta = fst::kDelta,
         int32_t max_states = -1, bool use_log = false) -> bool {
        bool debug_location = false;
        fst::ArcSort(in_out,
                     fst::ILabelCompare<fst::StdArc>());  // improves speed.

        if (use_log) {
          return DeterminizeStarInLog(in_out, delta, &debug_location,
                                      max_states);
        } else {
          fst::VectorFst<fst::StdArc> det_fst;

          bool status = fst::DeterminizeStar(*in_out, &det_fst, delta,
                                             &debug_location, max_states);
          *in_out = det_fst;
          return status;
        }
      },
      py::arg("in_out"), py::arg("delta") = fst::kDelta,
      py::arg("max_states") = -1, py::arg("use_log") = false,
      kFstDeterminizeStarDoc);
}

}  // namespace kaldifst
