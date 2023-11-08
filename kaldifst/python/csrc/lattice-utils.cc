// kaldifst/python/csrc/lattice-utils.cc
//
// Copyright (c)  2021-2023  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/csrc/lattice-utils.h"

#include "kaldifst/python/csrc/lattice-utils.h"
namespace kaldifst {

static constexpr const char *kLatticeScaleDoc = R"doc(
Return a diagonal scale matrix with specified LM scale ``lmwt``
and AM scale ``amwt``.

Note that the diagonal matrix is a list of list, e.g.,
``[[lmwt, 0], [0, acwt]]``.

Args:
  lmwt:
    Scale for language model scores.
  acwt:
    Scale for acoustic likelihoods.

Returns:
 - A list-of-list containing ``[[lmwt, 0], [0, acwt]]``.

**Example 1: Return a matrix with lmwt=0.1, acwt=10.0**

.. literalinclude:: ./code/lattice_scale/ex.py
   :language: python
   :linenos:
   :caption: Return a matrix with lmwt=0.1, acwt=10.0
)doc";
static constexpr const char *kScaleLatticeDoc = R"doc(
Scales the pairs of weights in ``LatticeWeight`` by
viewing the pair ``(a, b)`` as a 2-vector and pre-multiplying by
the 2x2 matrix in "scale".  E.g. typically scale would equal

.. code-block:: bash

     [ 1   0;
       0  acwt ]

if we want to scale the acoustics by ``acwt``.

  - out_value1 = scale[0][0]*in_value1 + scale[0][1]*in_value2
  - out_value2 = scale[1][0]*in_value1 + scale[1][1]*in_value2

Args:
  scale:
    A list-of-list containing the weight
  in_out:
    The lattice is changed in-place.
Returns:
  Return ``None``.

Example 1: Use a diagonal scale ``[[0.1, 0], [0, 10]]``

.. literalinclude:: ./code/scale_lattice/ex1.py
   :language: python
   :linenos:
   :caption: Scale a lattice.

.. figure:: ./code/scale_lattice/before-scale.svg
    :alt: before-scale.svg
    :align: center
    :figwidth: 600px

    Before scale.

.. figure:: ./code/scale_lattice/after-scale.svg
    :alt: after-scale.svg
    :align: center
    :figwidth: 600px

    After scale using ``[[0.1, 0], [0, 10]]``.

Example 2: Use a non-diagonal scale ``[[0.1, 1], [0.5, 10]]``

.. literalinclude:: ./code/scale_lattice/ex2.py
   :language: python
   :linenos:
   :caption: Scale a lattice.

.. figure:: ./code/scale_lattice/before-scale-2.svg
    :alt: before-scale-2.svg
    :align: center
    :figwidth: 600px

    Before scale.

.. figure:: ./code/scale_lattice/after-scale-2.svg
    :alt: after-scale-2.svg
    :align: center
    :figwidth: 600px

    After scale using ``[[0.1, 1], [0.5, 10]]``.
)doc";

void PybindLatticeUtils(py::module *m) {
  m->def("lattice_scale", &fst::LatticeScale, py::arg("lmwt"), py::arg("acwt"),
         kLatticeScaleDoc);
  m->def("scale_lattice", &fst::ScaleLattice<fst::LatticeWeight, double>,
         py::arg("scale"), py::arg("in_out"), kScaleLatticeDoc);
}

}  // namespace kaldifst
