// kaldifst/python/csrc/fstarcsort.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstminimize.h"

#include "fst/minimize.h"
#include "fst/shortest-distance.h"
#include "kaldifst/csrc/log.h"
#include "kaldifst/python/csrc/fstarcsort.h"

static constexpr const char *kFstMinimizeDoc = R"doc(
This operation performs the minimization of deterministic weighted automata
and transducers.

See also `<https://www.openfst.org/twiki/bin/view/FST/MinimizeDoc>`_.

.. caution::

   The FST is modified in-place.

Args:
  in_out:
    An acceptor or a transducer. It is modified in-place.
  delta:
    Comparison/quantization delta
  allow_nondet: type = bool, default = false
    True to minimize non-deterministic FSTs

Returns:
  Return ``None``.

**Example 1: Minimize an acceptor**

.. literalinclude:: ./code/minimize/acceptor.py
   :language: python
   :linenos:
   :caption: Minimize an acceptor

.. figure:: ./code/minimize/acceptor.svg
    :alt: acceptor.svg
    :align: center
    :figwidth: 600px

    Visualization of acceptor.svg (before minimize)

.. figure:: ./code/minimize/acceptor-minimized.svg
    :alt: acceptor-minimized.svg
    :align: center
    :figwidth: 600px

    Visualization of acceptor-minimized.svg (after minimize)

**Example 2: Minimize a transducer**

.. literalinclude:: ./code/minimize/transducer.py
   :language: python
   :linenos:
   :caption: Minimize a transducer

.. figure:: ./code/minimize/transducer.svg
    :alt: transducer.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer.svg (before minimize)

.. figure:: ./code/minimize/transducer-minimized.svg
    :alt: transducer-minimized.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer-minimized.svg (after minimize)
)doc";

namespace kaldifst {

void PybindFstMinimize(py::module &m) {  // NOLINT
  m.def(
      "minimize",
      [](fst::MutableFst<fst::StdArc> *in_out, double delta = 1e-6,
         bool allow_nondet = false) {
        fst::Minimize<fst::StdArc>(in_out, nullptr, delta, allow_nondet);
      },
      py::arg("in_out"), py::arg("delta") = 1e-6,
      py::arg("allow_nondet") = false, kFstMinimizeDoc);
}

}  // namespace kaldifst
