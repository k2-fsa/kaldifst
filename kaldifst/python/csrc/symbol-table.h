// kaldifst/python/csrc/symbol-table.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFST_PYTHON_CSRC_SYMBOL_TABLE_H_
#define KALDIFST_PYTHON_CSRC_SYMBOL_TABLE_H_

#include "kaldifst/python/csrc/kaldifst.h"

namespace kaldifst {

void PybindSymbolTable(py::module &m);

}  // namespace kaldifst

#endif  // KALDIFST_PYTHON_CSRC_SYMBOL_TABLE_H_
