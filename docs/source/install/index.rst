Installation
============

You can use the following methods to install `kaldifst`_ on Windows (Win64/Win32),
Linux (x86/x86_64/aarch64), and macOS (x86_64/arm64).

``Python >= 3.8`` is known to work.

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

   git clone https://github.com/k2-fsa/kaldifst
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

It should print the version of `kaldifst`_, e.g., ``1.7.4``.
