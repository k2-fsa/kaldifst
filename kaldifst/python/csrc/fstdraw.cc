// kaldifst/python/csrc/fstdraw.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstdraw.h"

#include "kaldifst/csrc/log.h"

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

void PybindFstDraw(py::module &m) { PybindFstDraw<fst::StdArc>(m); }

}  // namespace kaldifst
