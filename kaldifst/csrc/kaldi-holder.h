// kaldifst/csrc/kaldi-holder.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/kaldi-holder.h

// Copyright 2009-2011     Microsoft Corporation
//                2016     Johns Hopkins University (author: Daniel Povey)
//                2016     Xiaohui Zhang
//
#ifndef KALDIFST_CSRC_KALDI_HOLDER_H_
#define KALDIFST_CSRC_KALDI_HOLDER_H_

#include <string>

namespace kaldifst {

/// We define a Token (not a typedef, just a word) as a nonempty, printable,
/// whitespace-free std::string.  The binary and text formats here are the same
/// (newline-terminated) and as such we don't bother with the binary-mode
/// headers.
class TokenHolder;

// In SequentialTableReaderScriptImpl and RandomAccessTableReaderScriptImpl, for
// cases where the scp contained 'range specifiers' (things in square brackets
// identifying parts of objects like matrices), use this function to separate
// the input string 'rxfilename_with_range' (e.g "1.ark:100[1:2,2:10]") into the
// data_rxfilename (e.g. "1.ark:100") and the optional range specifier which
// will be everything inside the square brackets.  It returns true if everything
// seems OK, and false if for example the string contained more than one '['.
// This function should only be called if 'line' ends in ']', otherwise it is an
// error.
bool ExtractRangeSpecifier(const std::string &rxfilename_with_range,
                           std::string *data_rxfilename, std::string *range);

}  // namespace kaldifst

#include "kaldifst/csrc/kaldi-holder-inl.h"

#endif  // KALDIFST_CSRC_KALDI_HOLDER_H_
