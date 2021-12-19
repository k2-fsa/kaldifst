// kaldifst/csrc/io-funcs-inl.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/base/io-funcs-inl.h

// Copyright 2009-2011  Microsoft Corporation;  Saarland University;
//                      Jan Silovsky;   Yanmin Qian;
//                      Johns Hopkins University (Author: Daniel Povey)
//                2016  Xiaohui Zhang
#ifndef KALDIFST_CSRC_IO_FUNCS_INL_H_
#define KALDIFST_CSRC_IO_FUNCS_INL_H_

#include <iostream>

namespace kaldifst {

// Initialize an opened stream for writing by writing an optional binary
// header and modifying the floating-point precision.
inline void InitKaldiOutputStream(std::ostream &os, bool binary) {
  // This does not throw exceptions (does not check for errors).
  if (binary) {
    os.put('\0');
    os.put('B');
  }
  // Note, in non-binary mode we may at some point want to mess with
  // the precision a bit.
  // 7 is a bit more than the precision of float..
  if (os.precision() < 7) os.precision(7);
}

/// Initialize an opened stream for reading by detecting the binary header and
// setting the "binary" value appropriately.
inline bool InitKaldiInputStream(std::istream &is, bool *binary) {
  // Sets the 'binary' variable.
  // Throws exception in the very unusual situation that stream
  // starts with '\0' but not then 'B'.

  if (is.peek() == '\0') {  // seems to be binary
    is.get();
    if (is.peek() != 'B') {
      return false;
    }
    is.get();
    *binary = true;
    return true;
  } else {
    *binary = false;
    return true;
  }
}

}  // namespace kaldifst

#endif  // KALDIFST_CSRC_IO_FUNCS_INL_H_
