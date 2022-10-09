.. _read-write-ark-scp:

Read and write ark/scp files from Kaldi
=======================================

In this section, we demonstrate

  - how to generate ark/scp files that can be read by `Kaldi`_
  - how to read ark/scp files from Kaldi

Generate ark/scp files
----------------------

We provide ``kaldifst.VectorFstWriter`` to save FSTs to ark/scp files.
The following is an example:

.. literalinclude:: ./code/writer.py
   :language: python
   :linenos:
   :caption: Create ark/scp files with ``kaldifst.VectorFstWriter``.

The above code creates two files

.. code-block:: bash

  $ ls -lh lat.scp lat.ark
  -rw-r--r-- 1 kuangfangjun root 452 Oct  8 23:18 lat.ark
  -rw-r--r-- 1 kuangfangjun root  32 Oct  8 23:18 lat.scp

You can use `fstcopy <https://github.com/kaldi-asr/kaldi/blob/master/src/fstbin/fstcopy.cc>`_
from `Kaldi`_ to view them:

.. code-block:: bash

  $ fstcopy scp:lat.scp ark,t:-
  fstcopy scp:lat.scp ark,t:-
  lat1
  0       1       a       1       0.5
  1       2       b       2       0.8
  2       0.25

  lat2
  0       1       a       A       0.5
  1       2       b       B       0.8
  2       0.25

  LOG (fstcopy[5.5.1035~1-3dd90]:main():fstcopy.cc:71) Copied 2 FSTs.

Read ark/scp files sequentially
-------------------------------

We provide ``kaldifst.SequentialVectorFstReader`` to read FSTs from ark/scp files
sequentially. The following is an example:

.. literalinclude:: ./code/sequential-reader.py
   :language: python
   :linenos:
   :caption: Read ark/scp files sequentially with ``kaldifst.SequentialVectorFstReader``.

The above code produces the following output:

.. code-block::

  key lat1
  value
   0      1      a      1      0.5
  1      2      b      2      0.8
  2      0.25

  key lat2
  value
   0      1      a      A      0.5
  1      2      b      B      0.8
  2      0.25

Read ark/scp files randomly
---------------------------

We provide ``kaldifst.RandomAccessVectorFstReader`` to read FSTs from ark/scp files
randomly. The following is an example:

.. literalinclude:: ./code/random-access-reader.py
   :language: python
   :linenos:
   :caption: Read ark/scp files randomly with ``kaldifst.RandomAccessVectorFstReader``.

The above code produces the following output:

.. code-block::

  lat1 0      1      a      1      0.5
  1      2      b      2      0.8
  2      0.25

  lat2 0      1      a      A      0.5
  1      2      b      B      0.8
  2      0.25
