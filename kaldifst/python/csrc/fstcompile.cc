// kaldifst/python/csrc/fstcompile.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstcompile.h"

#include "fst/script/compile.h"
#include "kaldifst/csrc/log.h"

namespace kaldifst {
struct FstCompileParams {
  // Input in acceptor format
  bool acceptor = false;

  // Output arc type
  std::string arc_type = "standard";

  // Output FST type
  std::string fst_type = "vector";

  // Input label symbol table
  // Can be either a path to the SymbolTable
  // or an instance of SymbolTable
  py::object isymbols;
  // Output label symbol table
  py::object osymbols;

  // State label symbol table
  py::object ssymbols;

  // Store input label symbol table with FST
  bool keep_isymbols = false;

  // Store output label symbol table with FST
  bool keep_osymbols = false;

  // Do not renumber input states
  bool keep_state_numbering = false;

  // Allow negative labels (not recommended; may cause conflicts)
  bool allow_negative_labels = false;
};

py::object FstCompileImpl(const FstCompileParams &params,
                          const std::string &str) {
  using fst::SymbolTable;
  using fst::SymbolTableTextOptions;

  std::stringstream ss(str);

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

  std::unique_ptr<fst::script::FstClass> fst(fst::script::CompileFstInternal(
      ss, "", params.fst_type, params.arc_type, isyms.get(), osyms.get(),
      ssyms.get(), params.acceptor, params.keep_isymbols, params.keep_osymbols,
      params.keep_state_numbering, params.allow_negative_labels));

  if (params.arc_type == "standard") {
    const fst::StdFst *_ans = fst->GetFst<fst::StdArc>();
    std::unique_ptr<fst::StdFst> ans(_ans->Copy());
    return py::cast(std::move(ans));
  } else {
    KALDIFST_ERR << "Unsupported arc type: " << params.arc_type;
  }
}

void PybindFstCompile(py::module &m) {
  m.def(
      "compile",
      [](const std::string &s, bool acceptor = false,
         const std::string &arc_type = "standard",
         const std::string &fst_type = "vector",
         py::object isymbols = py::none(), py::object osymbols = py::none(),
         py::object ssymbols = py::none(), bool keep_isymbols = false,
         bool keep_osymbols = false, bool keep_state_numbering = false,
         bool allow_negative_labels = false) -> py::object {
        FstCompileParams params;

        params.acceptor = acceptor;
        params.arc_type = arc_type;
        params.fst_type = fst_type;
        params.isymbols = isymbols;
        params.osymbols = osymbols;
        params.ssymbols = ssymbols;
        params.keep_isymbols = keep_isymbols;
        params.keep_osymbols = keep_osymbols;
        params.keep_state_numbering = keep_state_numbering;
        params.allow_negative_labels = allow_negative_labels;

        return FstCompileImpl(params, s);
      },
      py::arg("s"), py::arg("acceptor") = false,
      py::arg("arc_type") = "standard", py::arg("fst_type") = "vector",
      py::arg("isymbols") = py::none(), py::arg("osymbols") = py::none(),
      py::arg("ssymbols") = py::none(), py::arg("keep_isymbols") = false,
      py::arg("keep_osymbols") = false, py::arg("keep_state_numbering") = false,
      py::arg("allow_negative_labels") = false);
}

}  // namespace kaldifst
