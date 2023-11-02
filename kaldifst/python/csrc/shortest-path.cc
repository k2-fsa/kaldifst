// kaldifst/python/csrc/shortest-path.cc
//
// Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)
#include "kaldifst/python/csrc/shortest-path.h"

#include "kaldifst/csrc/lattice-weight.h"

namespace kaldifst {

static constexpr const char *kShortestPathDoc = R"doc(
This operation produces an FST containing the n-shortest paths in the input FST.

The n -shortest paths are the n -lowest weight paths w.r.t. the natural semiring
order. The single path that can be read from the ith of at most n transitions
leaving the initial state of the resulting FST is the ith shortest path.

See also `<https://www.openfst.org/twiki/bin/view/FST/ShortestPathDoc>`_.

Args:
  n:
    Size of n-best.
  unique:
     Return only distinct strings. (NB: must be acceptor; epsilons
     treated as regular symbols)
Returns:
  Return a VectorFst containing n paths.

**Example 1: StdVectorFst**

.. literalinclude:: ./code/shortest_path/ex1.py
   :language: python
   :linenos:
   :caption: ShortestPath for a StdVectorFst

.. figure:: ./code/shortest_path/vector-fst.svg
    :alt: vector-fst.svg
    :align: center
    :figwidth: 600px

    Visualization of vector-fst.svg

.. figure:: ./code/shortest_path/vector-fst-1best.svg
    :alt: vector-fst-1best.svg
    :align: center
    :figwidth: 600px

    Visualization of vector-fst-1best.svg

.. figure:: ./code/shortest_path/vector-fst-2best.svg
    :alt: vector-fst-2best.svg
    :align: center
    :figwidth: 600px

    Visualization of vector-fst-2best.svg

.. figure:: ./code/shortest_path/vector-fst-3best.svg
    :alt: vector-fst-3best.svg
    :align: center
    :figwidth: 600px

    Visualization of vector-fst-3best.svg

**Example 2: Lattice**

.. literalinclude:: ./code/shortest_path/ex2.py
   :language: python
   :linenos:
   :caption: ShortestPath for a Lattice

.. figure:: ./code/shortest_path/lattice.svg
    :alt: lattice.svg
    :align: center
    :figwidth: 600px

    Visualization of lattice.svg

.. figure:: ./code/shortest_path/lattice-nbest.svg
    :alt: lattice-nbest.svg
    :align: center
    :figwidth: 600px

    Visualization of lattice-nbest.svg
)doc";

void PybindShortestPath(py::module *m) {
  PybindShortestPath<fst::StdArc>(m, kShortestPathDoc);
  PybindShortestPath<fst::LatticeArc>(m, kShortestPathDoc);
}

}  // namespace kaldifst
