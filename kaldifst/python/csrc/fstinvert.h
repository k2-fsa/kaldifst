// kaldifst/python/csrc/fstinvert.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_FSTINVERT_H_
#define KALDIFST_PYTHON_CSRC_FSTINVERT_H_

#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindFstInvert(py::module &m);  // NOLINT

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_FSTINVERT_H_
