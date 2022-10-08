// kaldifst/python/csrc/fstext-utils.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/csrc/fstext-utils.h"

#include "kaldifst/python/csrc/fstarcsort.h"
#include "kaldifst/python/csrc/fstext-utils.h"

static constexpr const char *kFstMinimizeEncodedDoc = R"doc(
Minimizes after encoding; applicable to all FSTs.

Its implementation is from `Kaldi`_:

.. code-block:: cpp

    Map(fst, QuantizeMapper<Arc>(delta));
    EncodeMapper<Arc> encoder(kEncodeLabels | kEncodeWeights, ENCODE);
    Encode(fst, &encoder);
    internal::AcceptorMinimize(fst);
    Decode(fst, encoder);

See also `<https://github.com/kaldi-asr/kaldi/blob/master/src/fstbin/fstminimizeencoded.cc>`_.

.. caution::

   The FST is modified in-place.

Args:
  in_out:
    An acceptor or a transducer. It is modified in-place.
  delta:
    Comparison/quantization delta

Returns:
  Return ``None``.

**Example 1: Minimize encode an acceptor**

.. literalinclude:: ./code/minimize_encoded/acceptor.py
   :language: python
   :linenos:
   :caption: Minimize encode an acceptor

.. figure:: ./code/minimize_encoded/acceptor.svg
    :alt: acceptor.svg
    :align: center
    :figwidth: 600px

    Visualization of acceptor.svg (before minimize_encoded)

.. figure:: ./code/minimize_encoded/acceptor-minimize-encoded.svg
    :alt: acceptor-minimize-encoded.svg
    :align: center
    :figwidth: 600px

    Visualization of acceptor-minimize-encoded.svg (after minimize_encoded)

**Example 2: Minimize encode a transducer**

.. literalinclude:: ./code/minimize_encoded/transducer.py
   :language: python
   :linenos:
   :caption: Minimize encode a transducer

.. figure:: ./code/minimize_encoded/transducer.svg
    :alt: transducer.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer.svg (before minimize_encoded)

.. figure:: ./code/minimize_encoded/transducer-minimize-encoded.svg
    :alt: transducer-minimize-encoded.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer-minimize-encoded.svg (after minimize encoded)
)doc";

namespace kaldifst {

void PybindFstExtUtils(py::module &m) {  // NOLINT
  m.def(
      "minimize_encoded",
      [](fst::MutableFst<fst::StdArc> *in_out, double delta = fst::kDelta) {
        fst::MinimizeEncoded(in_out, delta);
      },
      py::arg("in_out"), py::arg("delta") = fst::kDelta,
      kFstMinimizeEncodedDoc);
}

}  // namespace kaldifst
