// kaldifst/python/csrc/fstreverse.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstreverse.h"

#include <memory>
#include <utility>

#include "fst/reverse.h"

static constexpr const char *kFstReverseDoc = R"doc(
This operation reverses an FST. If A transduces string x to y with weight a,
then the reverse of A transduces the reverse of x to the reverse of y
with weight a.Reverse().

See also `<https://www.openfst.org/twiki/bin/view/FST/ReverseDoc>`_.

Args:
  ifst:
    The input FST.
  require_superinitial:
    True to create a superinitial state.

Returns:
  Return the reversed FST.

**Example 1 Revert an acceptor (require_superinitial=True)**

.. literalinclude:: ./code/reverse/ex1.py
   :language: python
   :linenos:
   :caption: Revert an acceptor using require_superinitial=True

.. figure:: ./code/reverse/acceptor-before-reverse.svg
    :alt: acceptor-before-reverse.svg
    :align: center
    :figwidth: 600px

    Visualization of acceptor-before-reverse.svg

.. figure:: ./code/reverse/acceptor-after-reverse.svg
    :alt: acceptor-after-reverse.svg
    :align: center
    :figwidth: 600px

    Visualization of acceptor-after-reverse.svg using ``require_superinitial=True``

**Example 2 Revert an acceptor (require_superinitial=False)**

.. literalinclude:: ./code/reverse/ex2.py
   :language: python
   :linenos:
   :caption: Revert an acceptor using require_superinitial=False

.. figure:: ./code/reverse/acceptor-before-reverse-2.svg
    :alt: acceptor-before-reverse-2.svg
    :align: center
    :figwidth: 600px

    Visualization of acceptor-before-reverse-2.svg

.. figure:: ./code/reverse/acceptor-after-reverse-2.svg
    :alt: acceptor-after-reverse-2.svg
    :align: center
    :figwidth: 600px

    Visualization of acceptor-after-reverse-2.svg using ``require_superinitial=False``

**Example 3 Revert a transducer  (require_superinitial=False)**

.. literalinclude:: ./code/reverse/ex3.py
   :language: python
   :linenos:
   :caption: Revert a transducer using require_superinitial=True

.. figure:: ./code/reverse/transducer-before-reverse.svg
    :alt: transducer-before-reverse.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer-before-reverse.svg

.. figure:: ./code/reverse/transducer-after-reverse.svg
    :alt: transducer-after-reverse.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer-after-reverse.svg using ``require_superinitial=True``

**Example 4 Revert a transducer (require_superinitial=False)**

.. literalinclude:: ./code/reverse/ex4.py
   :language: python
   :linenos:
   :caption: Revert a transducer using require_superinitial=False

.. figure:: ./code/reverse/transducer-before-reverse-2.svg
    :alt: transducer-before-reverse-2.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer-before-reverse-2.svg

.. figure:: ./code/reverse/transducer-after-reverse-2.svg
    :alt: transducer-after-reverse-2.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer-after-reverse-2.svg using ``require_superinitial=False``
)doc";

namespace kaldifst {

void PybindFstReverse(py::module &m) {  // NOLINT
  m.def(
      "reverse",
      [](const fst::StdFst &ifst, bool require_superinitial = true)
          -> std::unique_ptr<fst::StdVectorFst> {
        auto ans = std::make_unique<fst::StdVectorFst>();
        fst::Reverse(ifst, ans.get(), require_superinitial);
        return std::move(ans);
      },
      py::arg("ifst"), py::arg("require_superinitial") = true, kFstReverseDoc);
}

}  // namespace kaldifst
