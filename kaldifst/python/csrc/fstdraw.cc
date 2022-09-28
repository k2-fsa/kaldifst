// kaldifst/python/csrc/fstdraw.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstdraw.h"

#include <memory>
#include <string>

#include "kaldifst/csrc/log.h"

static constexpr const char *kFstDrawDoc = R"doc(
Prints FSTs in dot text format.

.. hint::

  You can use the Python package ``graphviz`` to visualize it.

  You can also post the dot format output to
  `<https://dreampuf.github.io/GraphvizOnline/>`_ to visualize it within
  your browser. You can also share the URL to others.

Args:
  fst:
    The FST to be printed.
  acceptor:
    Input in acceptor format
  isymbols:
    Input label symbol table
  osymbols:
    Output label symbol table
  ssymbols:
    State label symbol table
  numeric:
    Print numeric labels
  precision:
    Set precision (number of char/float)
  float_format:
    Floating-point format, one of: "e", "f", or "g"
  show_weight_one:
    Print/draw arc weights and final weights equal to Weight::One()
  title:
    Set figure title
  portrait:
    Portrait mode (def: landscape)
  vertical:
    Draw bottom-to-top instead of left-to-right
  fontsize:
    Set fontsize
  height:
    Set height
  width:
    Set width
  nodesep:
    Set minimum separation between nodes (see dot documentation)
  ranksep:
    Set minimum separation between ranks (see dot documentation)
  allow_negative_labels:
    Allow negative labels (not recommended; may cause conflicts)

Returns:
  Return a string.

**Example 1: Draw a transducer**

.. literalinclude:: ./code/draw/ex.py
   :language: python
   :linenos:
   :caption: Draw a transducer.

.. literalinclude:: ./code/draw/fst_dot.txt
   :linenos:
   :caption: fst_dot.txt

.. figure:: ./code/draw/fst.svg
    :alt: fst.svg
    :align: center
    :figwidth: 600px

    Visualization of fst.svg
)doc";

namespace kaldifst {

std::string FstDrawImpl(const fst::script::FstClass &fst,
                        const FstDrawParams &params) {
  using fst::SymbolTable;
  using fst::SymbolTableTextOptions;

  std::ostringstream os;

  const SymbolTableTextOptions opts(params.allow_negative_labels);

  std::unique_ptr<const SymbolTable> isyms;
  if (!params.isymbols.is_none()) {
    if (py::isinstance<py::str>(params.isymbols)) {
      isyms.reset(SymbolTable::ReadText(py::str(params.isymbols), opts));
    } else {
      isyms.reset(py::cast<SymbolTable *>(params.isymbols)->Copy());
    }
    KALDIFST_ASSERT(isyms);
  }

  std::unique_ptr<const SymbolTable> osyms;
  if (!params.osymbols.is_none()) {
    if (py::isinstance<py::str>(params.osymbols)) {
      osyms.reset(SymbolTable::ReadText(py::str(params.osymbols), opts));
    } else {
      osyms.reset(py::cast<SymbolTable *>(params.osymbols)->Copy());
    }
    KALDIFST_ASSERT(osyms);
  }

  std::unique_ptr<const SymbolTable> ssyms;
  if (!params.ssymbols.is_none()) {
    if (py::isinstance<py::str>(params.ssymbols)) {
      ssyms.reset(SymbolTable::ReadText(py::str(params.ssymbols), opts));
    } else {
      ssyms.reset(py::cast<SymbolTable *>(params.ssymbols)->Copy());
    }
    KALDIFST_ASSERT(ssyms);
  }

  if (!isyms && !params.numeric && fst.InputSymbols()) {
    isyms.reset(fst.InputSymbols()->Copy());
  }

  if (!osyms && !params.numeric && fst.OutputSymbols()) {
    osyms.reset(fst.OutputSymbols()->Copy());
  }

  std::string dest = "";
  fst::script::DrawFst(
      fst, isyms.get(), osyms.get(), ssyms.get(), params.acceptor, params.title,
      params.width, params.height, params.portrait, params.vertical,
      params.ranksep, params.nodesep, params.fontsize, params.precision,
      params.float_format, params.show_weight_one, &os, dest);
  return os.str();
}

void PybindFstDraw(py::module &m) {  // NOLINT
  PybindFstDraw<fst::StdArc>(m, kFstDrawDoc);
}

}  // namespace kaldifst
