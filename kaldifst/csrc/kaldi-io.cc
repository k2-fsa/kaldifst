// kaldifst/csrc/kaldi-io.cc
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/kaldi-io.cc

// Copyright 2009-2011  Microsoft Corporation;  Jan Silovsky
//                2016  Xiaohui Zhang
#include "kaldifst/csrc/kaldi-io.h"

#include <string.h>

#include "kaldifst/csrc/kaldi-table.h"
#include "kaldifst/csrc/log.h"
namespace kaldifst {

OutputType ClassifyWxfilename(const std::string &filename) {
  const char *c = filename.c_str();
  size_t length = filename.length();
  char first_char = c[0],
       last_char = (length == 0 ? '\0' : c[filename.length() - 1]);

  // if 'filename' is "" or "-", return kStandardOutput.
  if (length == 0 || (length == 1 && first_char == '-'))
    return kStandardOutput;
  else if (first_char == '|')
    return kPipeOutput;  // An output pipe like "|blah".
  else if (isspace(first_char) || isspace(last_char) || last_char == '|') {
    return kNoOutput;  // Leading or trailing space: can't interpret this.
                       // Final '|' would represent an input pipe, not an
                       // output pipe.
  } else if ((first_char == 'a' || first_char == 's') &&
             strchr(c, ':') != NULL &&
             (ClassifyWspecifier(filename, NULL, NULL, NULL) != kNoWspecifier ||
              ClassifyRspecifier(filename, NULL, NULL) != kNoRspecifier)) {
    // e.g. ark:something or scp:something... this is almost certainly a
    // scripting error, so call it an error rather than treating it as a file.
    // In practice in modern kaldi scripts all (r,w)filenames begin with "ark"
    // or "scp", even though technically speaking options like "b", "t", "s" or
    // "cs" can appear before the ark or scp, like "b,ark".  For efficiency,
    // and because this code is really just a nicety to catch errors earlier
    // than they would otherwise be caught, we only call those extra functions
    // for filenames beginning with 'a' or 's'.
    return kNoOutput;
  } else if (isdigit(last_char)) {
    // This could be a file, but we have to see if it's an offset into a file
    // (like foo.ark:4314328), which is not allowed for writing (but is
    // allowed for reaching).  This eliminates some things which would be
    // valid UNIX filenames but are not allowed by Kaldi.  (Even if we allowed
    // such filenames for writing, we woudln't be able to correctly read them).
    const char *d = c + length - 1;
    while (isdigit(*d) && d > c) d--;
    if (*d == ':') return kNoOutput;
    // else it could still be a filename; continue to the next check.
  }

  // At this point it matched no other pattern so we assume a filename, but we
  // check for internal '|' as it's a common source of errors to have pipe
  // commands without the pipe in the right place.  Say that it can't be
  // classified.
  if (strchr(c, '|') != NULL) {
    KALDIFST_WARN << "Trying to classify wxfilename with pipe symbol in the"
                     " wrong place (pipe without | at the beginning?): "
                  << filename;
    return kNoOutput;
  }
  return kFileOutput;  // It matched no other pattern: assume it's a filename.
}

InputType ClassifyRxfilename(const std::string &filename) {
  const char *c = filename.c_str();
  size_t length = filename.length();
  char first_char = c[0],
       last_char = (length == 0 ? '\0' : c[filename.length() - 1]);

  // if 'filename' is "" or "-", return kStandardInput.
  if (length == 0 || (length == 1 && first_char == '-')) {
    return kStandardInput;
  } else if (first_char == '|') {
    return kNoInput;  // An output pipe like "|blah": not
                      // valid for input.
  } else if (last_char == '|') {
    return kPipeInput;
  } else if (isspace(first_char) || isspace(last_char)) {
    return kNoInput;  // We don't allow leading or trailing space in a filename.
  } else if ((first_char == 'a' || first_char == 's') &&
             strchr(c, ':') != NULL &&
             (ClassifyWspecifier(filename, NULL, NULL, NULL) != kNoWspecifier ||
              ClassifyRspecifier(filename, NULL, NULL) != kNoRspecifier)) {
    // e.g. ark:something or scp:something... this is almost certainly a
    // scripting error, so call it an error rather than treating it as a file.
    // In practice in modern kaldi scripts all (r,w)filenames begin with "ark"
    // or "scp", even though technically speaking options like "b", "t", "s" or
    // "cs" can appear before the ark or scp, like "b,ark".  For efficiency,
    // and because this code is really just a nicety to catch errors earlier
    // than they would otherwise be caught, we only call those extra functions
    // for filenames beginning with 'a' or 's'.
    return kNoInput;
  } else if (isdigit(last_char)) {
    const char *d = c + length - 1;
    while (isdigit(*d) && d > c) d--;
    if (*d == ':')
      return kOffsetFileInput;  // Filename is like
                                // some_file:12345
    // otherwise it could still be a filename; continue to the next check.
  }

  // At this point it matched no other pattern so we assume a filename, but
  // we check for '|' as it's a common source of errors to have pipe
  // commands without the pipe in the right place.  Say that it can't be
  // classified in this case.
  if (strchr(c, '|') != NULL) {
    KALDIFST_WARN << "Trying to classify rxfilename with pipe symbol in the"
                     " wrong place (pipe without | at the end?): "
                  << filename;
    return kNoInput;
  }
  return kFileInput;  // It matched no other pattern: assume it's a filename.
}

}  // namespace kaldifst
