// kaldifst/python/csrc/shortest-path.cc
//
// Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)
#include "kaldifst/python/csrc/shortest-path.h"

#include "kaldifst/csrc/lattice-weight.h"

namespace kaldifst {

static constexpr const char *kShortestPathDoc = R"doc(

)doc";

void PybindShortestPath(py::module *m) {
  PybindShortestPath<fst::StdArc>(m, kShortestPathDoc);
  PybindShortestPath<fst::LatticeArc>(m, kShortestPathDoc);
}

}  // namespace kaldifst
