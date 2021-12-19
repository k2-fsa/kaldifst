// kaldifst/csrc/parse-options.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/parse-options.cc

// Copyright 2009-2011  Karel Vesely;  Microsoft Corporation;
//                      Saarland University (Author: Arnab Ghoshal);
// Copyright 2012-2013  Johns Hopkins University (Author: Daniel Povey);
//                      Frantisek Skala;  Arnab Ghoshal
// Copyright 2013       Tanel Alumae
#include "kaldifst/csrc/parse-options.h"

#include <cstring>

#include "kaldifst/csrc/log.h"

namespace kaldifst {

// We currently do not support any other options.
enum ShellType { kBash = 0 };

// This can be changed in the code if it ever does need to be changed (as it's
// unlikely that one compilation of this tool-set would use both shells).
static ShellType kShellType = kBash;

// Returns true if we need to escape a string before putting it into
// a shell (mainly thinking of bash shell, but should work for others)
// This is for the convenience of the user so command-lines that are
// printed out by ParseOptions::Read (with --print-args=true) are
// paste-able into the shell and will run. If you use a different type of
// shell, it might be necessary to change this function.
// But it's mostly a cosmetic issue as it basically affects how
// the program echoes its command-line arguments to the screen.
static bool MustBeQuoted(const std::string &str, ShellType st) {
  // Only Bash is supported (for the moment).
  KALDIFST_ASSERT(st == kBash && "Invalid shell type.");

  const char *c = str.c_str();
  if (*c == '\0') {
    return true;  // Must quote empty string
  } else {
    const char *ok_chars[2];

    // These seem not to be interpreted as long as there are no other "bad"
    // characters involved (e.g. "," would be interpreted as part of something
    // like a{b,c}, but not on its own.
    ok_chars[kBash] = "[]~#^_-+=:.,/";

    // Just want to make sure that a space character doesn't get automatically
    // inserted here via an automated style-checking script, like it did before.
    KALDIFST_ASSERT(!strchr(ok_chars[kBash], ' '));

    for (; *c != '\0'; c++) {
      // For non-alphanumeric characters we have a list of characters which
      // are OK. All others are forbidden (this is easier since the shell
      // interprets most non-alphanumeric characters).
      if (!isalnum(*c)) {
        const char *d;
        for (d = ok_chars[st]; *d != '\0'; d++)
          if (*c == *d) break;
        // If not alphanumeric or one of the "ok_chars", it must be escaped.
        if (*d == '\0') return true;
      }
    }
    return false;  // The string was OK. No quoting or escaping.
  }
}

// Returns a quoted and escaped version of "str"
// which has previously been determined to need escaping.
// Our aim is to print out the command line in such a way that if it's
// pasted into a shell of ShellType "st" (only bash for now), it
// will get passed to the program in the same way.
static std::string QuoteAndEscape(const std::string &str, ShellType st) {
  // Only Bash is supported (for the moment).
  KALDIFST_ASSERT(st == kBash && "Invalid shell type.");

  // For now we use the following rules:
  // In the normal case, we quote with single-quote "'", and to escape
  // a single-quote we use the string: '\'' (interpreted as closing the
  // single-quote, putting an escaped single-quote from the shell, and
  // then reopening the single quote).
  char quote_char = '\'';
  const char *escape_str = "'\\''";  // e.g. echo 'a'\''b' returns a'b

  // If the string contains single-quotes that would need escaping this
  // way, and we determine that the string could be safely double-quoted
  // without requiring any escaping, then we double-quote the string.
  // This is the case if the characters "`$\ do not appear in the string.
  // e.g. see http://www.redhat.com/mirrors/LDP/LDP/abs/html/quotingvar.html
  const char *c_str = str.c_str();
  if (strchr(c_str, '\'') && !strpbrk(c_str, "\"`$\\")) {
    quote_char = '"';
    escape_str = "\\\"";  // should never be accessed.
  }

  char buf[2];
  buf[1] = '\0';

  buf[0] = quote_char;
  std::string ans = buf;
  const char *c = str.c_str();
  for (; *c != '\0'; c++) {
    if (*c == quote_char) {
      ans += escape_str;
    } else {
      buf[0] = *c;
      ans += buf;
    }
  }
  buf[0] = quote_char;
  ans += buf;
  return ans;
}

// static function
std::string ParseOptions::Escape(const std::string &str) {
  return MustBeQuoted(str, kShellType) ? QuoteAndEscape(str, kShellType) : str;
}

}  // namespace kaldifst
