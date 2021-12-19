// kaldifst/csrc/io-funcs.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/base/io-funcs.h

// Copyright 2009-2011  Microsoft Corporation;  Saarland University;
//                      Jan Silovsky;   Yanmin Qian
//                2016  Xiaohui Zhang
#ifndef KALDIFST_CSRC_IO_FUNCS_H_
#define KALDIFST_CSRC_IO_FUNCS_H_

#include "kaldifst/csrc/io-funcs-inl.h"

namespace kaldifst {

/// InitKaldiOutputStream initializes an opened stream for writing by writing an
/// optional binary header and modifying the floating-point precision; it will
/// typically not be called by users directly.
inline void InitKaldiOutputStream(std::ostream &os, bool binary);

/// InitKaldiInputStream initializes an opened stream for reading by detecting
/// the binary header and setting the "binary" value appropriately;
/// It will typically not be called by users directly.
inline bool InitKaldiInputStream(std::istream &is, bool *binary);

}  // namespace kaldifst
#endif  // KALDIFST_CSRC_IO_FUNCS_H_
