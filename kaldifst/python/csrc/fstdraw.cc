// kaldifst/python/csrc/fstdraw.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstdraw.h"

#include "kaldifst/csrc/log.h"

namespace kaldifst {

std::string FstDrawParams::ToString() const {
  std::ostringstream os;
  os << "isymbols: " << isymbols << "\n";
  os << "osymbols: " << osymbols << "\n";
  os << "ssymbols: " << ssymbols << "\n";
  os << "numeric: " << numeric << "\n";
  os << "precision: " << precision << "\n";
  os << "float_format: " << float_format << "\n";
  os << "show_weight_one: " << show_weight_one << "\n";
  os << "title: " << title << "\n";
  os << "portrait: " << portrait << "\n";
  os << "vertical: " << vertical << "\n";
  os << "fontsize: " << fontsize << "\n";
  os << "height: " << height << "\n";
  os << "width: " << width << "\n";
  os << "nodesep: " << nodesep << "\n";
  os << "ranksep: " << ranksep << "\n";
  os << "allow_negative_labels: " << allow_negative_labels << "\n";
  return os.str();
}

std::string FstDrawImpl(const fst::script::FstClass &fst,
                        const FstDrawParams &params) {
  std::ostringstream os;

  const fst::SymbolTableTextOptions opts(params.allow_negative_labels);

  std::unique_ptr<const fst::SymbolTable> isyms;
  if (!params.isymbols.empty() && !params.numeric) {
    isyms.reset(fst::SymbolTable::ReadText(params.isymbols, opts));
    KALDIFST_ASSERT(!isyms);
  }

  std::unique_ptr<const fst::SymbolTable> osyms;
  if (!params.osymbols.empty() && !params.numeric) {
    osyms.reset(fst::SymbolTable::ReadText(params.osymbols, opts));
    KALDIFST_ASSERT(!osyms);
  }

  std::unique_ptr<const fst::SymbolTable> ssyms;
  if (!params.ssymbols.empty() && !params.numeric) {
    ssyms.reset(fst::SymbolTable::ReadText(params.ssymbols));
    KALDIFST_ASSERT(!ssyms);
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

void PybindFstDraw(py::module &m) {
  py::class_<FstDrawParams>(m, "FstDrawParams")
      .def(py::init<>())
      .def_readwrite("acceptor", &FstDrawParams::acceptor)
      .def_readwrite("isymbols", &FstDrawParams::isymbols)
      .def_readwrite("osymbols", &FstDrawParams::osymbols)
      .def_readwrite("ssymbols", &FstDrawParams::ssymbols)
      .def_readwrite("numeric", &FstDrawParams::numeric)
      .def_readwrite("precision", &FstDrawParams::precision)
      .def_readwrite("float_format", &FstDrawParams::float_format)
      .def_readwrite("show_weight_one", &FstDrawParams::show_weight_one)
      .def_readwrite("title", &FstDrawParams::title)
      .def_readwrite("portrait", &FstDrawParams::portrait)
      .def_readwrite("vertical", &FstDrawParams::vertical)
      .def_readwrite("fontsize", &FstDrawParams::fontsize)
      .def_readwrite("height", &FstDrawParams::height)
      .def_readwrite("width", &FstDrawParams::width)
      .def_readwrite("nodesep", &FstDrawParams::nodesep)
      .def_readwrite("ranksep", &FstDrawParams::ranksep)
      .def_readwrite("allow_negative_labels",
                     &FstDrawParams::allow_negative_labels)
      .def("__str__", [](const FstDrawParams &self) -> std::string {
        return self.ToString();
      });

  PybindFstDraw<fst::StdArc>(m);
}

}  // namespace kaldifst
