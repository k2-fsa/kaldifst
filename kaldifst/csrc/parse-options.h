// kaldifst/csrc/parse-options.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/parse-options.h

// Copyright 2009-2011  Karel Vesely;  Microsoft Corporation;
//                      Saarland University (Author: Arnab Ghoshal);
// Copyright 2012-2013  Frantisek Skala;  Arnab Ghoshal

#ifndef KALDIFST_CSRC_PARSE_OPTIONS_H_
#define KALDIFST_CSRC_PARSE_OPTIONS_H_

#include <string>

namespace kaldifst {

class ParseOptions {
 public:
  /// The following function will return a possibly quoted and escaped
  /// version of "str", according to the current shell.  Currently
  /// this is just hardwired to bash.  It's useful for debug output.
  static std::string Escape(const std::string &str);
};

}  // namespace kaldifst
#endif  // KALDIFST_CSRC_PARSE_OPTIONS_H_
