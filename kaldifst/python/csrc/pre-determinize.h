// kaldifst/python/csrc/predeterminize.h
//
// Copyright (c)  2021-2023  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_PRE_DETERMINIZE_H_
#define KALDIFST_PYTHON_CSRC_PRE_DETERMINIZE_H_

#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindPreDeterminize(py::module &m);  // NOLINT

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_PRE_DETERMINIZE_H_
