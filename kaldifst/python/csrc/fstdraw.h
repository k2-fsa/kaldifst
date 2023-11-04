// kaldifst/python/csrc/fstdraw.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_FSTDRAW_H_
#define KALDIFST_PYTHON_CSRC_FSTDRAW_H_

#include <memory>
#include <string>

#include "fst/script/draw.h"
#include "kaldifst/csrc/lattice-weight.h"
#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindFstDraw(py::module &m);  // NOLINT

struct FstDrawParams {
  // Input in acceptor format
  bool acceptor = false;

  // Input label symbol table
  py::object isymbols;

  // Output label symbol table
  py::object osymbols;

  // State label symbol table
  py::object ssymbols;

  // Print numeric labels
  bool numeric = false;

  // Set precision (number of char/float)
  int32_t precision = 5;

  // Floating-point format, one of: "e", "f", or "g"
  std::string float_format = "g";
  // Print/draw arc weights and final weights equal to Weight::One()
  bool show_weight_one = false;

  // Set figure title
  std::string title = "";

  // Portrait mode (def: landscape)
  bool portrait = false;

  // Draw bottom-to-top instead of left-to-right
  bool vertical = false;

  // Set fontsize
  int32_t fontsize = 14;

  // Set height
  double height = 11;

  // Set width
  double width = 8.5;

  // Set minimum separation between nodes (see dot documentation)
  double nodesep = 0.25;

  // Set minimum separation between ranks (see dot documentation)
  double ranksep = 0.40;

  // Allow negative labels (not recommended; may cause conflicts)
  bool allow_negative_labels = false;
};

template <typename A>
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
  fst::FstDrawer<A> a(
      *fst.GetFst<A>(), isyms.get(), osyms.get(), ssyms.get(), params.acceptor,
      params.title, params.width, params.height, params.portrait,
      params.vertical, params.ranksep, params.nodesep, params.fontsize,
      params.precision, params.float_format, params.show_weight_one);
  a.Draw(&os, dest);
  return os.str();
}

template <typename A>
void PybindFstDraw(py::module &m,  // NOLINT
                   const char *doc = "") {
  using PyClass = fst::Fst<A>;

  m.def(
      "draw",
      [](const PyClass &fst, bool acceptor = false,
         py::object isymbols = py::none(), py::object osymbols = py::none(),
         py::object ssymbols = py::none(), bool numeric = false,
         int32_t precision = 5, const std::string &float_format = "g",
         bool show_weight_one = false, const std::string &title = "",
         bool portrait = false, bool vertical = false, int32_t fontsize = 14,
         double height = 11, double width = 8.5, double nodesep = 0.25,
         double ranksep = 0.40,
         bool allow_negative_labels = false) -> std::string {
        auto _fst = fst::script::FstClass(fst);
        FstDrawParams params;
        params.acceptor = acceptor;
        params.isymbols = isymbols;
        params.osymbols = osymbols;
        params.ssymbols = ssymbols;
        params.numeric = numeric;
        params.precision = precision;
        params.float_format = float_format;
        params.show_weight_one = show_weight_one;
        params.title = title;
        params.portrait = portrait;
        params.vertical = vertical;
        params.fontsize = fontsize;
        params.height = height;
        params.width = width;
        params.nodesep = nodesep;
        params.ranksep = ranksep;
        params.allow_negative_labels = allow_negative_labels;
        return FstDrawImpl<A>(_fst, params);
      },
      py::arg("fst"), py::arg("acceptor") = false,
      py::arg("isymbols") = py::none(), py::arg("osymbols") = py::none(),
      py::arg("ssymbols") = py::none(), py::arg("numeric") = false,
      py::arg("precision") = 5, py::arg("float_format") = "g",
      py::arg("show_weight_one") = false, py::arg("title") = "",
      py::arg("portrait") = false, py::arg("vertical") = false,
      py::arg("fontsize") = 14, py::arg("height") = 11, py::arg("width") = 8.5,
      py::arg("nodesep") = 0.25, py::arg("ranksep") = 0.40,
      py::arg("allow_negative_labels") = false, doc);
}

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_FSTDRAW_H_
