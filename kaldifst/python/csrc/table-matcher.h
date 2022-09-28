// kaldifst/python/csrc/table-matcher.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_TABLE_MATCHER_H_
#define KALDIFST_PYTHON_CSRC_TABLE_MATCHER_H_

#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindFstTableCompose(py::module &m);  // NOLINT

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_TABLE_MATCHER_H_
