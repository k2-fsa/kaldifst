// kaldifst/python/csrc/fstrmepsilon.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/fstrmepsilon.h"

#include <string>

#include "fst/script/getters.h"
#include "fst/script/rmepsilon.h"
#include "kaldifst/csrc/log.h"

namespace kaldifst {

void PybindFstRmEpsilon(py::module &m) {  // NOLINT
  m.def(
      "rmepsilon",
      [](fst::MutableFst<fst::StdArc> *in_out, bool connect = true,
         double delta = 1e-6, int64_t nstate = fst::kNoStateId,
         const std::string &queue_type = "auto",
         const std::string &weight = "") {
        namespace s = fst::script;
        using fst::script::WeightClass;

        const auto weight_threshold =
            weight.empty() ? WeightClass::Zero(fst::TropicalWeight::Type())
                           : WeightClass(fst::TropicalWeight::Type(), weight);

        fst::QueueType fst_queue_type;
        if (!s::GetQueueType(queue_type, &fst_queue_type)) {
          KALDIFST_ERR << "Unknown or unsupported queue type: " << queue_type;
        }

        const s::RmEpsilonOptions opts(fst_queue_type, connect,
                                       weight_threshold, nstate, delta);

        s::internal::RmEpsilon(in_out, opts);
      },
      py::arg("in_out"), py::arg("connect") = true, py::arg("delta") = 1e-6,
      py::arg("nstate") = -1, py::arg("queue_type") = "auto",
      py::arg("weight") = "");
}

}  // namespace kaldifst
