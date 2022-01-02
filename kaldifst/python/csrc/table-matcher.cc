// kaldifst/python/csrc/table-matcher.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/table-matcher.h"

#include "kaldifst/csrc/log.h"
#include "kaldifst/csrc/table-matcher.h"

namespace kaldifst {

template <class Arc>
static void PybindFstTableComposeTpl(py::module &m) {
  m.def(
      "compose",
      [](const fst::Fst<Arc> &fst1, const fst::Fst<Arc> &fst2,
         const std::string &match_side = "left",
         const std::string &compose_filter = "sequence",
         bool connect = true) -> std::unique_ptr<fst::MutableFst<Arc>> {
        fst::TableComposeOptions opts;
        opts.connect = connect;

        if (match_side == "left") {
          opts.table_match_type = fst::MATCH_OUTPUT;
        } else if (match_side == "right") {
          opts.table_match_type = fst::MATCH_INPUT;
        } else {
          KALDIFST_ERR << "Invalid match-side option: " << match_side;
        }

        if (compose_filter == "alt_sequence") {
          opts.filter_type = fst::ALT_SEQUENCE_FILTER;
        } else if (compose_filter == "auto") {
          opts.filter_type = fst::AUTO_FILTER;
        } else if (compose_filter == "match") {
          opts.filter_type = fst::MATCH_FILTER;
        } else if (compose_filter == "sequence") {
          opts.filter_type = fst::SEQUENCE_FILTER;
        } else {
          KALDIFST_ERR << "Invalid compose-filter option: " << compose_filter;
        }

        // Checks if <fst1> is olabel sorted and <fst2> is ilabel sorted.
        if (fst1.Properties(fst::kOLabelSorted, true) == 0) {
          KALDIFST_WARN << "The first FST is not olabel sorted.";
        }
        if (fst2.Properties(fst::kILabelSorted, true) == 0) {
          KALDIFST_WARN << "The second FST is not ilabel sorted.";
        }

        auto composed_fst = std::make_unique<fst::VectorFst<Arc>>();

        fst::TableCompose(fst1, fst2, composed_fst.get(), opts);
        return std::move(composed_fst);
      },
      py::arg("fst1"), py::arg("fst2"), py::arg("match_side") = "left",
      py::arg("compose_filter") = "sequence", py::arg("connect") = "true");
}

void PybindFstTableCompose(py::module &m) {
  PybindFstTableComposeTpl<fst::StdArc>(m);
}

}  // namespace kaldifst
