// kaldifst/csrc/text-utils.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/text-utils.h

// Copyright 2009-2011  Saarland University;  Microsoft Corporation

#include <limits>
#include <string>
#include <type_traits>
#include <vector>

#ifndef KALDIFST_CSRC_TEXT_UTILS_H_
#define KALDIFST_CSRC_TEXT_UTILS_H_

#ifdef _MSC_VER
#define KALDIFST_STRTOLL(cur_cstr, end_cstr) _strtoi64(cur_cstr, end_cstr, 10);
#else
#define KALDIFST_STRTOLL(cur_cstr, end_cstr) strtoll(cur_cstr, end_cstr, 10);
#endif

namespace kaldifst {

/// Split a string using any of the single character delimiters.
/// If omit_empty_strings == true, the output will contain any
/// nonempty strings after splitting on any of the
/// characters in the delimiter.  If omit_empty_strings == false,
/// the output will contain n+1 strings if there are n characters
/// in the set "delim" within the input string.  In this case
/// the empty string is split to a single empty string.
void SplitStringToVector(const std::string &full, const char *delim,
                         bool omit_empty_strings,
                         std::vector<std::string> *out);

/// Converts a string into an integer via strtoll and returns false if there was
/// any kind of problem (i.e. the string was not an integer or contained extra
/// non-whitespace junk, or the integer was too large to fit into the type it is
/// being converted into).  Only sets *out if everything was OK and it returns
/// true.
template <class Int>
bool ConvertStringToInteger(const std::string &str, Int *out) {
  static_assert(std::is_integral<Int>::value, "");
  const char *this_str = str.c_str();
  char *end = NULL;
  errno = 0;
  int64_t i = KALDIFST_STRTOLL(this_str, &end);
  if (end != this_str)
    while (isspace(*end)) end++;
  if (end == this_str || *end != '\0' || errno != 0) return false;
  Int iInt = static_cast<Int>(i);
  if (static_cast<int64_t>(iInt) != i ||
      (i < 0 && !std::numeric_limits<Int>::is_signed)) {
    return false;
  }
  *out = iInt;
  return true;
}

/// Returns true if "token" is nonempty, and all characters are
/// printable and whitespace-free.
bool IsToken(const std::string &token);

// CharToString prints the character in a human-readable form, for debugging.
std::string CharToString(const char &c);

/// Removes leading and trailing white space from the string, then splits on the
/// first section of whitespace found (if present), putting the part before the
/// whitespace in "first" and the rest in "rest".  If there is no such space,
/// everything that remains after removing leading and trailing whitespace goes
/// in "first".
void SplitStringOnFirstSpace(const std::string &line, std::string *first,
                             std::string *rest);

}  // namespace kaldifst

#endif  // KALDIFST_CSRC_TEXT_UTILS_H_
