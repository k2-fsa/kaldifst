// kaldifst/python/csrc/table-matcher.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/table-matcher.h"

#include "kaldifst/csrc/log.h"
#include "kaldifst/csrc/table-matcher.h"

static constexpr const char *kFstArcSortDoc = R"doc(
Compose two FSTs.

This operation computes the composition of two transducers. If A transduces
string x to y with weight a and B transduces y to z with weight b, then their
composition transduces string x to z with weight a ⊗ b.

See also `<https://www.openfst.org/twiki/bin/view/FST/ComposeDoc>`_

.. caution::

  The output labels of the first transducer or the input labels of the
  second transducer must be ``sorted``.


Args:
  fst1:
    The first FST.
  fst2:
    The second FST.
  match_side:
    Defaults to left.
  compose_filter:
    Composition filter, one of: "alt_sequence", "auto", "match",
    "no_match", "null", "sequence", "trivial".
  connect:
    Trim output.
Returns:
  Return the composition result.

**Example 1: Compose two transducers.**

.. literalinclude:: ./code/compose/ex.py
   :language: python
   :linenos:
   :caption: Compose two transducers

.. figure:: ./code/compose/a.svg
    :alt: a.svg
    :align: center
    :figwidth: 600px

    The first transducer a.

.. figure:: ./code/compose/b.svg
    :alt: b.svg
    :align: center
    :figwidth: 600px

    The second transducer b

.. figure:: ./code/compose/c.svg
    :alt: c.svg
    :align: center
    :figwidth: 600px

    The composition result c.
)doc";

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
      py::arg("compose_filter") = "sequence", py::arg("connect") = true,
      kFstArcSortDoc);
}

void PybindFstTableCompose(py::module &m) {
  PybindFstTableComposeTpl<fst::StdArc>(m);
}

}  // namespace kaldifst
