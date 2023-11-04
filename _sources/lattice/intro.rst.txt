.. _introduction_to_lattice:

Introduction to Lattice
=======================
Arcs in a :ref:`lattice_class` are of type :ref:`lattice_arc_class`, whose weight
is of type :ref:`lattice_weight_class`.

A :ref:`lattice_weight_class` has two parts:

  - ``value1``: It contains the graph cost
  - ``value2``: It contains the acoustic cost


The following code demonstrates how to create and visualize a lattice.

.. literalinclude:: ./code/ex1.py
   :language: python
   :linenos:
   :caption: Create, print, and visualize a lattice.

.. figure:: ./code/lattice.svg
    :alt: lattice.svg
    :align: center
    :figwidth: 600px

    lattice.svg generated from the above code.

``1:2/0.1,0.2`` means:

  - The input label is 1
  - The output label is 2
  - The graph cost is 0.1
  - The acoustic cost is 0.2
