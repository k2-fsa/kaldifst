// kaldifst/python/csrc/fstdraw.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_FSTDRAW_H_
#define KALDIFST_PYTHON_CSRC_FSTDRAW_H_

#include "fst/script/draw.h"
#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindFstDraw(py::module &m);

struct FstDrawParams {
  // Input in acceptor format
  bool acceptor = false;

  // Input label symbol table
  std::string isymbols;

  // Output label symbol table
  std::string osymbols;

  // State label symbol table
  std::string ssymbols;

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
  std::string ToString() const;
};

std::string FstDrawImpl(const fst::script::FstClass &fst,
                        const FstDrawParams &params);

template <typename A>
void PybindFstDraw(py::module &m) {
  using PyClass = fst::Fst<A>;
  m.def(
      "draw",
      [](const PyClass &fst, const FstDrawParams &params) -> std::string {
        auto _fst = fst::script::FstClass(fst);
        return FstDrawImpl(_fst, params);
      },
      py::arg("fst"), py::arg("params") = FstDrawParams{});

  m.def(
      "draw",
      [](const PyClass &fst, bool acceptor = false,
         const std::string &isymbols = "", const std::string &osymbols = "",
         const std::string &ssymbols = "", bool numeric = false,
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
        return FstDrawImpl(_fst, params);
      },
      py::arg("fst"), py::arg("acceptor") = false, py::arg("isymbols") = "",
      py::arg("osymbols") = "", py::arg("ssymbols") = "",
      py::arg("numeric") = false, py::arg("precision") = 5,
      py::arg("float_format") = "g", py::arg("show_weight_one") = false,
      py::arg("title") = "", py::arg("portrait") = false,
      py::arg("vertical") = false, py::arg("fontsize") = 14,
      py::arg("height") = 11, py::arg("width") = 8.5, py::arg("nodesep") = 0.25,
      py::arg("ranksep") = 0.40, py::arg("allow_negative_labels") = false);
}

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_FSTDRAW_H_
