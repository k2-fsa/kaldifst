// kaldifst/python/csrc/fstarcsort.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstarcsort.h"

#include "fst/arcsort.h"
#include "kaldifst/csrc/log.h"

namespace kaldifst {

void PybindFstArcSort(py::module &m) {
  m.def(
      "arcsort",
      [](fst::MutableFst<fst::StdArc> *in_out,
         const std::string &sort_type = "ilabel") {
        if (sort_type == "ilabel") {
          fst::ILabelCompare<fst::StdArc> icomp;
          fst::ArcSort(in_out, icomp);
        } else if (sort_type == "olabel") {
          fst::OLabelCompare<fst::StdArc> ocomp;
          fst::ArcSort(in_out, ocomp);
        } else {
          KALDIFST_ERR << "Unsupported sort_type " << sort_type
                       << ". Expect: ilabel or olabel.";
        }
      },
      py::arg("in_out"), py::arg("sort_type") = "ilabel");
}

}  // namespace kaldifst
