// kaldifst/csrc/kaldi-table.cc
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/kaldi-table.cc

// Copyright 2009-2011  Microsoft Corporation

#include "kaldifst/csrc/kaldi-table.h"

#include <cstring>
#include <string>
#include <vector>

#include "kaldifst/csrc/text-utils.h"
namespace kaldifst {

RspecifierType ClassifyRspecifier(const std::string &rspecifier,
                                  std::string *rxfilename,
                                  RspecifierOptions *opts) {
  // Examples
  // ark:rxfilename  ->  kArchiveRspecifier
  // scp:rxfilename  -> kScriptRspecifier
  //
  // We also allow the meaningless prefixes b, and t,
  // plus the options o (once), no (not-once),
  // s (sorted) and ns (not-sorted), p (permissive)
  // and np (not-permissive).
  // so the following would be valid:
  //
  // f, o, b, np, ark:rxfilename  ->  kArchiveRspecifier
  //
  // Examples:
  //
  // b, ark:rxfilename  ->  kArchiveRspecifier
  // t, ark:rxfilename  ->  kArchiveRspecifier
  // b, scp:rxfilename  -> kScriptRspecifier
  // t, no, s, scp:rxfilename  -> kScriptRspecifier
  // t, ns, scp:rxfilename  -> kScriptRspecifier

  // Improperly formed Rspecifiers will be classified as kNoRspecifier.

  if (rxfilename) rxfilename->clear();

  if (opts != NULL)
    *opts = RspecifierOptions();  // Make sure all the defaults are as in the
                                  // default constructor of the options class.

  size_t pos = rspecifier.find(':');
  if (pos == std::string::npos) return kNoRspecifier;

  if (isspace(*(rspecifier.rbegin()))) return kNoRspecifier;  // Trailing space
  // disallowed.

  std::string before_colon(rspecifier, 0, pos),
      after_colon(rspecifier, pos + 1);

  std::vector<std::string> split_first_part;  // Split part before ':' on ', '.
  SplitStringToVector(before_colon, ", ", false, &split_first_part);  // false==
  // don't omit empty strings between commas.

  RspecifierType rs = kNoRspecifier;

  for (size_t i = 0; i < split_first_part.size(); i++) {
    const std::string &str = split_first_part[i];  // e.g. "b", "t", "f", "ark",
    // "scp".
    const char *c = str.c_str();
    if (!strcmp(c, "b"))
      ;  // Ignore this option.  It's so we can use the same
    // specifiers for rspecifiers and wspecifiers.
    else if (!strcmp(c, "t"))
      ;  // Ignore this option too.
    else if (!strcmp(c, "o")) {
      if (opts) opts->once = true;
    } else if (!strcmp(c, "no")) {
      if (opts) opts->once = false;
    } else if (!strcmp(c, "p")) {
      if (opts) opts->permissive = true;
    } else if (!strcmp(c, "np")) {
      if (opts) opts->permissive = false;
    } else if (!strcmp(c, "s")) {
      if (opts) opts->sorted = true;
    } else if (!strcmp(c, "ns")) {
      if (opts) opts->sorted = false;
    } else if (!strcmp(c, "cs")) {
      if (opts) opts->called_sorted = true;
    } else if (!strcmp(c, "ncs")) {
      if (opts) opts->called_sorted = false;
    } else if (!strcmp(c, "bg")) {
      if (opts) opts->background = true;
    } else if (!strcmp(c, "ark")) {
      if (rs == kNoRspecifier)
        rs = kArchiveRspecifier;
      else
        return kNoRspecifier;  // Repeated or combined ark and scp options
      // invalid.
    } else if (!strcmp(c, "scp")) {
      if (rs == kNoRspecifier)
        rs = kScriptRspecifier;
      else
        return kNoRspecifier;  // Repeated or combined ark and scp options
      // invalid.
    } else {
      return kNoRspecifier;  // Could not interpret this option.
    }
  }
  if ((rs == kArchiveRspecifier || rs == kScriptRspecifier) &&
      rxfilename != NULL)
    *rxfilename = after_colon;
  return rs;
}

WspecifierType ClassifyWspecifier(const std::string &wspecifier,
                                  std::string *archive_wxfilename,
                                  std::string *script_wxfilename,
                                  WspecifierOptions *opts) {
  //  Examples:
  //  ark,t:wxfilename -> kArchiveWspecifier
  //  ark,b:wxfilename -> kArchiveWspecifier
  //  scp,t:rxfilename -> kScriptWspecifier
  //  scp,t:rxfilename -> kScriptWspecifier
  //  ark,scp,t:filename, wxfilename -> kBothWspecifier
  //  ark,scp:filename, wxfilename ->  kBothWspecifier
  //  Note we can include the flush option (f) or no-flush (nf)
  // anywhere: e.g.
  //  ark,scp,f:filename, wxfilename ->  kBothWspecifier
  // or:
  //  scp,t,nf:rxfilename -> kScriptWspecifier

  if (archive_wxfilename) archive_wxfilename->clear();
  if (script_wxfilename) script_wxfilename->clear();

  size_t pos = wspecifier.find(':');
  if (pos == std::string::npos) return kNoWspecifier;
  if (isspace(*(wspecifier.rbegin()))) return kNoWspecifier;  // Trailing space
  // disallowed.

  std::string before_colon(wspecifier, 0, pos),
      after_colon(wspecifier, pos + 1);

  std::vector<std::string> split_first_part;  // Split part before ':' on ', '.
  SplitStringToVector(before_colon, ", ", false, &split_first_part);  // false==
  // don't omit empty strings between commas.

  WspecifierType ws = kNoWspecifier;

  if (opts != NULL)
    *opts = WspecifierOptions();  // Make sure all the defaults are as in the
                                  // default constructor of the options class.

  for (size_t i = 0; i < split_first_part.size(); i++) {
    const std::string &str = split_first_part[i];  // e.g. "b", "t", "f", "ark",
    // "scp".
    const char *c = str.c_str();
    if (!strcmp(c, "b")) {
      if (opts) opts->binary = true;
    } else if (!strcmp(c, "f")) {
      if (opts) opts->flush = true;
    } else if (!strcmp(c, "nf")) {
      if (opts) opts->flush = false;
    } else if (!strcmp(c, "t")) {
      if (opts) opts->binary = false;
    } else if (!strcmp(c, "p")) {
      if (opts) opts->permissive = true;
    } else if (!strcmp(c, "ark")) {
      if (ws == kNoWspecifier)
        ws = kArchiveWspecifier;
      else
        return kNoWspecifier;  // We do not allow "scp, ark", only "ark,
      // scp".
    } else if (!strcmp(c, "scp")) {
      if (ws == kNoWspecifier)
        ws = kScriptWspecifier;
      else if (ws == kArchiveWspecifier)
        ws = kBothWspecifier;
      else
        return kNoWspecifier;  // repeated "scp" option: invalid.
    } else {
      return kNoWspecifier;  // Could not interpret this option.
    }
  }

  switch (ws) {
    case kArchiveWspecifier:
      if (archive_wxfilename) *archive_wxfilename = after_colon;
      break;
    case kScriptWspecifier:
      if (script_wxfilename) *script_wxfilename = after_colon;
      break;
    case kBothWspecifier:
      pos = after_colon.find(',');  // first comma.
      if (pos == std::string::npos) return kNoWspecifier;
      if (archive_wxfilename)
        *archive_wxfilename = std::string(after_colon, 0, pos);
      if (script_wxfilename)
        *script_wxfilename = std::string(after_colon, pos + 1);
      break;
    case kNoWspecifier:
    default:
      break;
  }
  return ws;
}

}  // namespace kaldifst
