// kaldifst/python/csrc/expanded-fst.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_EXPANDED_FST_H_
#define KALDIFST_PYTHON_CSRC_EXPANDED_FST_H_

#include "fst/expanded-fst.h"
#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindExpandedFst(py::module &m);

template <typename A>
void PybindExpandedFst(py::module &m, const std::string &class_name,
                       const std::string &class_help_doc = "") {
  using PyClass = fst::ExpandedFst<A>;
  using Parent = fst::Fst<A>;
  py::class_<PyClass, Parent>(m, class_name.c_str(), class_help_doc.c_str())
      .def_property_readonly("num_states", &PyClass::NumStates);
}

template <class Impl, class FST = fst::ExpandedFst<typename Impl::Arc>>
void PybindImplToExpandedFst(py::module &m, const std::string &class_name,
                             const std::string &class_help_doc = "") {
  using PyClass = fst::ImplToExpandedFst<Impl, FST>;
  using Parent = fst::ImplToFst<Impl, FST>;
  py::class_<PyClass, Parent>(m, class_name.c_str(), class_help_doc.c_str());
}

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_EXPANDED_FST_H_
