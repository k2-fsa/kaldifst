// kaldifst/python/csrc/fstconnect.cc
//
// Copyright (c)  20222 Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstconnect.h"

#include <utility>

#include "fst/script/connect.h"

namespace kaldifst {

static constexpr const char *kFstConnectDoc = R"doc(
This operation trims an FST, removing states and arcs that are not on
successful paths.

.. caution::

    The input FST is modified in-place.

See also `<https://www.openfst.org/twiki/bin/view/FST/ConnectDoc>`_

Args:
  in_out:
    The FST to be connected. Note it is modified in-place.

Returns:
  Return ``None``.

**Example 1: Connect a transducer**

.. literalinclude:: ./code/connect/ex.py
   :language: python
   :linenos:
   :caption: Connect a transducer

.. figure:: ./code/connect/fst.svg
    :alt: fst.svg
    :align: center
    :figwidth: 600px

    Visualization of fst.svg (before connect)

.. figure:: ./code/connect/fst-connect.svg
    :alt: fst-connect.svg
    :align: center
    :figwidth: 600px

    Visualization of fst-connect.svg (after connect)

)doc";

template <typename A>
void PybindFstConnect(py::module &m,  // NOLINT
                      const char *doc = "") {
  using PyClass = fst::MutableFst<A>;
  m.def(
      "connect",
      [](PyClass &fst) {
        auto _fst = fst::script::MutableFstClass(fst);
        fst::script::Connect(&_fst);

        fst = std::move(*_fst.GetMutableFst<A>());
      },
      py::arg("in_out"), doc);
}

void PybindFstConnect(py::module &m) {  // NOLINT
  PybindFstConnect<fst::StdArc>(m, kFstConnectDoc);
}

}  // namespace kaldifst
