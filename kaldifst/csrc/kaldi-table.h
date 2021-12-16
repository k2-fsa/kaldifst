// kaldifst/csrc/kaldi-table.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/kaldi-table.h

// Copyright 2009-2011    Microsoft Corporation
//                2013    Johns Hopkins University (author: Daniel Povey)
#include <string>
namespace kaldifst {

// Documentation for "rspecifier"
// "rspecifier" describes how we read a set of objects indexed by keys.
// The possibilities are:
//
// ark:rxfilename
// scp:rxfilename
//
// We also allow various modifiers:
//   o   means the program will only ask for each key once, which enables
//       the reader to discard already-asked-for values.
//   s   means the keys are sorted on input (means we don't have to read till
//       eof if someone asked for a key that wasn't there).
//   cs  means that it is called in sorted order (we are generally asserting
//       this based on knowledge of how the program works).
//   p   means "permissive", and causes it to skip over keys whose corresponding
//       scp-file entries cannot be read. [and to ignore errors in archives and
//       script files, and just consider the "good" entries].
//       We allow the negation of the options above, as in no, ns, np,
//       but these aren't currently very useful (just equivalent to omitting the
//       corresponding option).
//       [any of the above options can be prefixed by n to negate them, e.g. no,
//       ns, ncs, np; but these aren't currently useful as you could just omit
//       the option].
//   bg means "background".  It currently has no effect for random-access
//   readers,
//       but for sequential readers it will cause it to "read ahead" to the next
//       value, in a background thread.  Recommended when reading larger objects
//       such as neural-net training examples, especially when you want to
//       maximize GPU usage.
//
//   b   is ignored [for scripting convenience]
//   t   is ignored [for scripting convenience]
//
//
//  So for instance the following would be a valid rspecifier:
//
//   "o, s, p, ark:gunzip -c foo.gz|"

struct RspecifierOptions {
  // These options only make a difference for the RandomAccessTableReader class.
  bool once;    // we assert that the program will only ask for each key once.
  bool sorted;  // we assert that the keys are sorted.
  bool called_sorted;  // we assert that the (HasKey(), Value() functions will
  // also be called in sorted order.  [this implies "once" but not vice versa].
  bool permissive;  // If "permissive", when reading from scp files it treats
  // scp files that can't be read as if the corresponding key were not there.
  // For archive files it will suppress errors getting thrown if the archive
  // is corrupted and can't be read to the end.
  bool background;  // For sequential readers, if the background option ("bg")
                    // is provided, it will read ahead to the next object in a
                    // background thread.
  RspecifierOptions()
      : once(false),
        sorted(false),
        called_sorted(false),
        permissive(false),
        background(false) {}
};

enum RspecifierType {
  kNoRspecifier,
  kArchiveRspecifier,
  kScriptRspecifier,
};

RspecifierType ClassifyRspecifier(const std::string &rspecifier,
                                  std::string *rxfilename,
                                  RspecifierOptions *opts);

// Documentation for "wspecifier"
// "wspecifier" describes how we write a set of objects indexed by keys.
// The basic, unadorned wspecifiers are as follows:
//
//  ark:wxfilename
//  scp:rxfilename
//  ark,scp:filename,wxfilename
//  ark,scp:filename,wxfilename
//
//
//  We also allow the following modifiers:
//  t means text mode.
//  b means binary mode.
//  f means flush the stream after writing each entry.
//   (nf means don't flush, and the default is not to flush).
//  p means permissive mode, when writing to an "scp" file only: will ignore
//     missing scp entries, i.e. won't write anything for those files but will
//     return success status).
//
//  So the following are valid wspecifiers:
//  ark,b,f:foo
//  "ark,b,b:| gzip -c > foo"
//  "ark,scp,t,nf:foo.ark,|gzip -c > foo.scp.gz"
//  ark,b:-
//
//  The meanings of rxfilename and wxfilename are as described in
//  kaldi-io.h (they are filenames but include pipes, stdin/stdout
//  and so on; filename is a regular filename.
//

//  The ark:wxfilename type of wspecifier instructs the class to
//  write directly to an archive.  For small objects (e.g. lists of ints),
//  the text archive format will generally be human readable with one line
//  per entry in the archive.
//
//  The type "scp:xfilename" refers to an scp file which should
//  already exist on disk, and tells us where to write the data for
//  each key (usually an actual file); each line of the scp file
//  would be:
//   key xfilename
//
//  The type ark,scp:filename,wxfilename means
//  we write both an archive and an scp file that specifies offsets into the
//  archive, with lines like:
//    key filename:12407
//  where the number is the byte offset into the file.
//  In this case we restrict the archive-filename to be an actual filename,
//  as we can't see a situation where an extended filename would make sense
//  for this (we can't fseek() in pipes).

enum WspecifierType {
  kNoWspecifier,
  kArchiveWspecifier,
  kScriptWspecifier,
  kBothWspecifier
};

struct WspecifierOptions {
  bool binary;
  bool flush;
  bool permissive;  // will ignore absent scp entries.
  WspecifierOptions() : binary(true), flush(false), permissive(false) {}
};

// ClassifyWspecifier returns the type of the wspecifier string,
// and (if pointers are non-NULL) outputs the extra information
// about the options, and the script and archive
// filenames.
WspecifierType ClassifyWspecifier(const std::string &wspecifier,
                                  std::string *archive_wxfilename,
                                  std::string *script_wxfilename,
                                  WspecifierOptions *opts);

}  // namespace kaldifst
