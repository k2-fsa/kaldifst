// kaldifst/python/csrc/fstarcsort.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstarcsort.h"

#include <string>

#include "fst/arcsort.h"
#include "kaldifst/csrc/log.h"

static constexpr const char *kFstArcSortDoc = R"doc(
Sort arcs of an FST **in-place**.

See also `<https://www.openfst.org/twiki/bin/view/FST/ArcSortDoc>`_

.. caution::

   The FST is modified in-place.

Args:
  in_out:
    An acceptor or a transducer. It is modified in-place.
  sort_type:
    Comparison method, one of "ilabel", "olabel"
Returns:
  Return ``None``.

**Example 1: Sort an acceptor**

.. literalinclude:: ./code/arcsort/acceptor.py
   :language: python
   :linenos:
   :caption: Sort an acceptor

.. figure:: ./code/arcsort/acceptor.svg
    :alt: acceptor.svg
    :align: center
    :figwidth: 600px

    Visualization of acceptor.svg (before sort)

.. figure:: ./code/arcsort/sorted.svg
    :alt: sorted.svg
    :align: center
    :figwidth: 600px

    Visualization of sorted.svg (after sort)

**Example 2: Sort a transducer by ilabel**

.. literalinclude:: ./code/arcsort/transducer1.py
   :language: python
   :linenos:
   :caption: Sort a transducer by ilabel

.. figure:: ./code/arcsort/transducer1.svg
    :alt: transducer1.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer1.svg (before sort)

.. figure:: ./code/arcsort/sorted-transducer-ilabel.svg
    :alt: sorted-transducer-ilabel.svg
    :align: center
    :figwidth: 600px

    Visualization of sorted-transducer-ilabel.svg (after sort)

**Example 3: Sort a transducer by olabel**

.. literalinclude:: ./code/arcsort/transducer2.py
   :language: python
   :linenos:
   :caption: Sort a transducer by olabel

.. figure:: ./code/arcsort/transducer2.svg
    :alt: transducer2.svg
    :align: center
    :figwidth: 600px

    Visualization of transducer2.svg (before sort)

.. figure:: ./code/arcsort/sorted-transducer-olabel.svg
    :alt: sorted-transducer-olabel.svg
    :align: center
    :figwidth: 600px

    Visualization of sorted-transducer-olabel.svg (after sort)
)doc";

namespace kaldifst {

void PybindFstArcSort(py::module &m) {  // NOLINT
  m.def(
      "arcsort",
      [](fst::MutableFst<fst::StdArc> *in_out,
         const std::string &sort_type = "ilabel") {
        if (sort_type == "ilabel") {
          fst::ILabelCompare<fst::StdArc> icomp;
          fst::ArcSort(in_out, icomp);
        } else if (sort_type == "olabel") {
          fst::OLabelCompare<fst::StdArc> ocomp;
          fst::ArcSort(in_out, ocomp);
        } else {
          KALDIFST_ERR << "Unsupported sort_type " << sort_type
                       << ". Expect: ilabel or olabel.";
        }
      },
      py::arg("in_out"), py::arg("sort_type") = "ilabel", kFstArcSortDoc);
}

}  // namespace kaldifst
