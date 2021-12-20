// kaldifst/python/csrc/fstcompile.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_FSTCOMPILE_H_
#define KALDIFST_PYTHON_CSRC_FSTCOMPILE_H_

#include "fst/script/compile.h"
#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindFstCompile(py::module &m);

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_FSTCOMPILE_H_
