Convert transducers
===================

This section demonstrates how to convert a transducer from `k2`_ to `OpenFST`_.

.. figure:: ./code/k2-transducer.svg
    :alt: k2-transducer.svg
    :align: center
    :figwidth: 600px

    A transducer in `k2`_.

.. figure:: ./code/openfst-transducer.svg
    :alt: openfst-transducer.svg
    :align: center
    :figwidth: 600px

    The above transducer converted to `OpenFST`_.
    Pay attention to the ``negative`` sign for numbers on the arcs.

.. literalinclude:: ./code/convert-transducer.py
   :language: python
   :linenos:
   :caption: Convert a transducer from `k2`_ to `OpenFST`_
