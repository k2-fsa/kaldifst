// kaldifst/python/csrc/fstconnect.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_FSTCONNECT_H_
#define KALDIFST_PYTHON_CSRC_FSTCONNECT_H_

#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindFstConnect(py::module &m);  // NOLINT

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_FSTCONNECT_H_
