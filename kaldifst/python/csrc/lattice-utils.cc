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

void PybindLatticeUtils(py::module *m) {
  m->def("lattice_scale", &fst::LatticeScale, py::arg("lmwt"), py::arg("acwt"),
         kLatticeScaleDoc);
}

}  // namespace kaldifst
