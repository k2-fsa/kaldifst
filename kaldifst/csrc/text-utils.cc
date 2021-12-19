// kaldifst/csrc/text-utils.cc
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/text-utils.cc

// Copyright 2009-2011  Saarland University;  Microsoft Corporation

#include "kaldifst/csrc/text-utils.h"

#include "kaldifst/csrc/log.h"

namespace kaldifst {

void SplitStringToVector(const std::string &full, const char *delim,
                         bool omit_empty_strings,
                         std::vector<std::string> *out) {
  size_t start = 0, found = 0, end = full.size();
  out->clear();
  while (found != std::string::npos) {
    found = full.find_first_of(delim, start);
    // start != end condition is for when the delimiter is at the end
    if (!omit_empty_strings || (found != start && start != end))
      out->push_back(full.substr(start, found - start));
    start = found + 1;
  }
}

bool IsToken(const std::string &token) {
  size_t l = token.length();
  if (l == 0) return false;
  for (size_t i = 0; i < l; i++) {
    unsigned char c = token[i];
    if ((!isprint(c) || isspace(c)) && (isascii(c) || c == (unsigned char)255))
      return false;
    // The "&& (isascii(c) || c == 255)" was added so that we won't reject
    // non-ASCII characters such as French characters with accents [except for
    // 255 which is "nbsp", a form of space].
  }
  return true;
}

std::string CharToString(const char &c) {
  char buf[20];
  if (std::isprint(c))
    std::snprintf(buf, sizeof(buf), "\'%c\'", c);
  else
    std::snprintf(buf, sizeof(buf), "[character %d]", static_cast<int>(c));
  return buf;
}

void SplitStringOnFirstSpace(const std::string &str, std::string *first,
                             std::string *rest) {
  const char *white_chars = " \t\n\r\f\v";
  typedef std::string::size_type I;
  const I npos = std::string::npos;
  I first_nonwhite = str.find_first_not_of(white_chars);
  if (first_nonwhite == npos) {
    first->clear();
    rest->clear();
    return;
  }
  // next_white is first whitespace after first nonwhitespace.
  I next_white = str.find_first_of(white_chars, first_nonwhite);

  if (next_white == npos) {  // no more whitespace...
    *first = std::string(str, first_nonwhite);
    rest->clear();
    return;
  }
  I next_nonwhite = str.find_first_not_of(white_chars, next_white);
  if (next_nonwhite == npos) {
    *first = std::string(str, first_nonwhite, next_white - first_nonwhite);
    rest->clear();
    return;
  }

  I last_nonwhite = str.find_last_not_of(white_chars);
  KALDIFST_ASSERT(last_nonwhite != npos);  // or coding error.

  *first = std::string(str, first_nonwhite, next_white - first_nonwhite);
  *rest = std::string(str, next_nonwhite, last_nonwhite + 1 - next_nonwhite);
}

}  // namespace kaldifst
