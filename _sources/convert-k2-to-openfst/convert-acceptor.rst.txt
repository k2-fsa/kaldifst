Convert acceptors
=================

This section demonstrates how to convert an acceptor from `k2`_ to `OpenFst`_.

.. figure:: ./code/k2-acceptor.svg
    :alt: k2-acceptor.svg
    :align: center
    :figwidth: 600px

    An acceptor in `k2`_.

.. figure:: ./code/openfst-acceptor.svg
    :alt: openfst-acceptor.svg
    :align: center
    :figwidth: 600px

    The above acceptor converted to `OpenFst`_.
    Pay attention to the ``negative`` sign for numbers on the arcs.

.. literalinclude:: ./code/convert-acceptor.py
   :language: python
   :linenos:
   :caption: Convert an acceptor from `k2`_ to `OpenFst`_
