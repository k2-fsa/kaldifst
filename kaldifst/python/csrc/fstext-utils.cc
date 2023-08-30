// kaldifst/python/csrc/fstext-utils.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/csrc/fstext-utils.h"

#include <tuple>
#include <utility>
#include <vector>

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

static constexpr const char *kMakeLinearAcceptorDoc = R"doc(
Creates unweighted linear acceptor from symbol sequence.

Args:
  labels:
    A list of symbol IDs.

Returns:
  Return a linear acceptor. Actually, it returns a transducer whose
  ilabel == olabel for each arc.

**Example 1: Build a linear acceptor**

.. literalinclude:: ./code/make_linear_acceptor/ex.py
   :language: python
   :linenos:
   :caption: Example of make_linear_acceptor()

.. figure:: ./code/make_linear_acceptor/acceptor.svg
    :alt: acceptor.svg
    :align: center
    :figwidth: 600px

    Visualization of acceptor.svg
)doc";

static constexpr const char *kEqualAlignDoc = R"doc(
Get a random linear path from an FST.

Args:
  ifst:
    The input fst.
  length:
    Path length of the output fst. If the ilabel of an arc is 0, then this
    arc does not contribute to the total path length of the output fst.
  rand_seed:
    A seed for random selecting arcs out of each state.
  num_retries:
    After trying this number but failed to generate a valid fst, it would
    return False.

Returns:
  Return a tuple containing:
    - succeeded, True if we successfully found a path.
    - fst, the output fst.

**Example 1: Equal align**

.. literalinclude:: ./code/equal_align/ex.py
   :language: python
   :linenos:
   :caption: Code for equal_align

.. figure:: ./code/equal_align/input.svg
    :alt: input.svg
    :align: center
    :figwidth: 600px

    Visualization of input.svg

.. figure:: ./code/equal_align/first.svg
    :alt: first.svg
    :align: center
    :figwidth: 600px

    Visualization of first.svg

.. figure:: ./code/equal_align/second.svg
    :alt: second.svg
    :align: center
    :figwidth: 600px

    Visualization of second.svg
)doc";

static constexpr const char *kGetLinearSymbolSequenceDoc = R"doc(
get_linear_symbol_sequence gets the symbol sequence from a linear FST.
If the FST is not just a linear sequence, it returns false.   If it is
a linear sequence (including the empty FST), it returns true.  In this
case it outputs the symbol

Args:
  fst:
    The input fst.
Returns:
  Return a tuple containing:

    - succeeded, bool, true if it succeeded.
    - isymbols_out, List[int], containing non-zero input symbols
    - osymbols_out, List[int], containing non-zero output symbols
    - total_weight_out, float, the total weight

**Example 1: get_linear_symbol_sequence**

.. literalinclude:: ./code/get_linear_symbol_sequence/ex.py
   :language: python
   :linenos:
   :caption: Code for get_linear_symbol_sequence
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

  m.def(
      "make_linear_acceptor",
      [](const std::vector<int32_t> &labels) -> fst::StdVectorFst {
        fst::StdVectorFst ans;
        fst::MakeLinearAcceptor(labels, &ans);
        return ans;
      },
      py::arg("labels"), kMakeLinearAcceptorDoc);

  m.def(
      "equal_align",
      [](const fst::StdVectorFst &ifst, int32_t length, int32_t rand_seed,
         int32_t num_retries = 10) -> std::pair<bool, fst::StdVectorFst> {
        fst::StdVectorFst ans;
        bool succeeded = EqualAlign(ifst, length, rand_seed, &ans, num_retries);
        return std::make_pair(succeeded, ans);
      },
      py::arg("ifst"), py::arg("length"), py::arg("rand_seed"),
      py::arg("num_retries") = 10, kEqualAlignDoc);
  m.def(
      "get_linear_symbol_sequence",
      [](const fst::StdFst &fst) -> std::tuple<bool, std::vector<int32_t>,
                                               std::vector<int32_t>, float> {
        std::vector<int32_t> isymbols_out;
        std::vector<int32_t> osymbols_out;
        float total_weight_out;
        fst::TropicalWeight w;

        bool succeeded =
            GetLinearSymbolSequence(fst, &isymbols_out, &osymbols_out, &w);
        total_weight_out = w.Value();

        return std::make_tuple(succeeded, isymbols_out, osymbols_out,
                               total_weight_out);
      },
      py::arg("fst"), kGetLinearSymbolSequenceDoc);
}

}  // namespace kaldifst
