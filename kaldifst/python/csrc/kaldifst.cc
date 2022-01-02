// kaldifst/python/csrc/kaldifst.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/kaldifst.h"

#include "kaldifst/python/csrc/arc.h"
#include "kaldifst/python/csrc/const-fst.h"
#include "kaldifst/python/csrc/expanded-fst.h"
#include "kaldifst/python/csrc/float-weight.h"
#include "kaldifst/python/csrc/fst.h"
#include "kaldifst/python/csrc/fstcompile.h"
#include "kaldifst/python/csrc/fstdraw.h"
#include "kaldifst/python/csrc/fstinfo.h"
#include "kaldifst/python/csrc/mutable-fst.h"
#include "kaldifst/python/csrc/symbol-table.h"
#include "kaldifst/python/csrc/table-matcher.h"
#include "kaldifst/python/csrc/vector-fst.h"

namespace kaldifst {

PYBIND11_MODULE(_kaldifst, m) {
  m.doc() = "Python wrapper for kaldifst";

  PybindFloatWeight(m);
  PybindArc(m);
  PybindSymbolTable(m);
  PybindFst(m);
  PybindExpandedFst(m);
  PybindMutableFst(m);
  PybindVectorFst(m);
  PybindConstFst(m);

  PybindFstInfo(m);
  PybindFstDraw(m);
  PybindFstCompile(m);

  PybindFstTableCompose(m);
}

}  // namespace kaldifst
