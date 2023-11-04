// kaldifst/python/csrc/shortest-path.h
//
// Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_SHORTEST_PATH_H_
#define KALDIFST_PYTHON_CSRC_SHORTEST_PATH_H_
#include <memory>

#include "fst/script/fst-class.h"
#include "fst/script/getters.h"
#include "fst/script/shortest-path.h"
#include "kaldifst/csrc/lattice-weight.h"
#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

template <typename Arc>
void PybindShortestPath(py::module *m, const char *doc = "") {
  using PyClass = fst::Fst<Arc>;

  m->def(
      "shortest_path",
      [](const PyClass &fst, int32_t n) -> fst::VectorFst<Arc> * {
        fst::VectorFst<Arc> ofst;
        fst::ShortestPath(fst, &ofst, n);
        // TopSort(&ofst);

        return ofst.Copy();
      },
      py::arg("fst"), py::arg("n") = 1, doc,
      py::return_value_policy::take_ownership);
}

void PybindShortestPath(py::module *m);

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_SHORTEST_PATH_H_
