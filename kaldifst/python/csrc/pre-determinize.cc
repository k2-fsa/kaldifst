// kaldifst/python/csrc/pre-determinize.cc
//
// Copyright (c)  2021-2023  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/pre-determinize.h"

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "kaldifst/csrc/log.h"
#include "kaldifst/csrc/pre-determinize.h"

static constexpr const char *kAddSelfLoopsDoc = R"doc(
AddSelfLoops is a function you will probably want to use alongside PreDeterminize,
to add self-loops to any FSTs that you compose on the left hand side of the one
modified by PreDeterminize.

This function inserts loops with "special symbols" [e.g. \#0, \#1] into an FST.
This is done at each final state and each state with non-epsilon output symbols on
at least one arc out of it.  This is to ensure that these symbols, when inserted into
the input side of an FST we will compose with on the right, can "pass through" this
FST.

At input, isyms and osyms must be vectors of the same size n, corresponding
to symbols that currently do not exist in 'fst'.  For each state in n that has
non-epsilon symbols on the output side of arcs leaving it, or which is a final state,
this function inserts n self-loops with unit weight and one of the n pairs
of symbols on its input and output.

.. caution::

    The input FST is modified in-place.

Args:
  ifst:
    The input FST.
  isyms:
    A list of input symbols.
  osyms:
    A list of output symbols. Must satisfy ``len(isyms) == len(osyms)``.

Returns:
  Return ``None``.

**Example 1: Add self loops to a transducer**

.. literalinclude:: ./code/add_self_loops/ex.py
   :language: python
   :linenos:
   :caption: Add self loops to a transducer.

.. figure:: ./code/add_self_loops/fst.svg
    :alt: fst.svg
    :align: center
    :figwidth: 600px

    Visualization of fst.svg (before connect)

.. figure:: ./code/add_self_loops/fst-add-self-loops.svg
    :alt: fst-add-self-loops.svg
    :align: center
    :figwidth: 600px

    Visualization of fst-add-self-loops.svg (after calling add_self_loops())
)doc";

namespace kaldifst {

template <typename Arc>
void PybindAddSelfLoops(py::module &m) {  // NOLINT
  using PyClass = fst::MutableFst<Arc>;
  using Label = typename Arc::Label;

  m.def(
      "add_self_loops",
      [](PyClass &fst, std::vector<Label> &isyms, std::vector<Label> &osyms) {
        AddSelfLoops(&fst, isyms, osyms);
      },
      py::arg("fst"), py::arg("isyms"), py::arg("osyms"), kAddSelfLoopsDoc);
}

void PybindPreDeterminize(py::module &m) {  // NOLINT
  PybindAddSelfLoops<fst::StdArc>(m);
}

}  // namespace kaldifst
