Installation
============

You can use the following methods to install `kaldifst`_ on Windows, Linux, and
macOS. ``Python >= 3.6`` is known to work.

Using conda
-----------

.. code-block:: bash

   conda install -c kaldifst kaldifst

To uninstall `kaldifst`_, use:

.. code-block:: bash

   conda remove kaldifst

Using pip
---------

.. code-block:: bash

   pip install --verbose kaldifst

or

.. code-block:: bash

   pip install --verbose -U git+https://github.com/kaldifst/kaldifst.git

To uninstall `kaldifst`_, use:

.. code-block:: bash

   pip uninstall kaldifst


From source
-----------

.. code-block::

   git clone https://github.com/csukuangfj/kaldifst
   cd kaldifst
   python3 setup.py install

To uninstall `kaldifst`_, use:

.. code-block:: bash

   pip uninstall kaldifst


Check kaldifst
--------------

To check that `kaldifst`_ was installed successfully, use:

.. code-block:: bash

   python3 -c "import kaldifst; print(kaldifst.__version__)"

It should print the version of `kaldifst`_, e.g., ``0.8``.
