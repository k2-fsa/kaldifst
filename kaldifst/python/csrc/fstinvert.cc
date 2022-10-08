// kaldifst/python/csrc/fstinvert.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstinvert.h"

#include "fst/invert.h"

static constexpr const char *kFstInvertDoc = R"doc(
This operation inverts the transduction corresponding to an FST by
exchanging the FST's input and output labels.

See also `<https://www.openfst.org/twiki/bin/view/FST/InvertDoc>`_

.. caution::

   The FST is modified in-place.

Args:
  in_out:
    A transducer. It is modified in-place.

Returns:
  Return ``None``.

**Example Invert a transducer**

.. literalinclude:: ./code/invert/ex.py
   :language: python
   :linenos:
   :caption: Invert a transducer

.. figure:: ./code/invert/before-invert.svg
    :alt: before-invert.svg
    :align: center
    :figwidth: 600px

    Visualization of before-invert.svg

.. figure:: ./code/invert/after-invert.svg
    :alt: after-invert.svg
    :align: center
    :figwidth: 600px

    Visualization of after-invert.svg
)doc";

namespace kaldifst {

void PybindFstInvert(py::module &m) {  // NOLINT
  m.def(
      "invert",  // in-place
      [](fst::MutableFst<fst::StdArc> *in_out) { fst::Invert(in_out); },
      py::arg("in_out"), kFstInvertDoc);
}

}  // namespace kaldifst
