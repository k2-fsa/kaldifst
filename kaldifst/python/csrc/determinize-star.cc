// kaldifst/python/csrc/determinize-star.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/csrc/determinize-star.h"

#include "fst/arcsort.h"
#include "kaldifst/csrc/fstext-utils.h"
#include "kaldifst/python/csrc/determinize-star.h"

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
          DeterminizeStarInLog(in_out, delta, &debug_location, max_states);
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
      R"(
Caution, the return status is un-intuitive: this function will return
false if determinization completed normally, and true if it was stopped
early by reaching the 'max-states' limit, and a partial FST was generated.
          )");
}

}  // namespace kaldifst
