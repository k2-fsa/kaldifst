// kaldifst/python/csrc/kaldifst.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/kaldifst.h"

#include "kaldifst/python/csrc/symbol-table.h"

namespace kaldifst {

PYBIND11_MODULE(_kaldifst, m) {
  m.doc() = "Python wrapper for kaldifst";

  PybindSymbolTable(m);
}

}  // namespace kaldifst
