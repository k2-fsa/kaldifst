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
      [](const PyClass &fst, int32_t n, bool unique,
         const std::string &queue_type_str = "auto") -> fst::VectorFst<Arc> * {
        fst::QueueType queue_type;

        if (!fst::script::GetQueueType(queue_type_str, &queue_type)) {
          fprintf(stderr,
                  "Unsupported queue_type: %s. Valid values: quto, fifo, lifo, "
                  "shortest, state, top.\n",
                  queue_type_str.c_str());
          return nullptr;
        }

        auto ifst = fst::script::FstClass(fst);
        fst::script::ShortestPathOptions opts(
            queue_type, n, unique, fst::kDelta,
            fst::script::WeightClass::Zero(ifst.WeightType()));

        fst::VectorFst<Arc> ofst;
        fst::script::internal::ShortestPath(fst, &ofst, opts);

        return ofst.Copy();
      },
      py::arg("fst"), py::arg("n") = 1, py::arg("unique") = false,
      py::arg("queue_type") = "auto"  // auto, fifo, lifo, shortest, state, top
  );
}

void PybindShortestPath(py::module *m);

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_SHORTEST_PATH_H_
