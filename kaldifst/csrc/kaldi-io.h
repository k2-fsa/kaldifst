// kaldifst/csrc/kaldi-io.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/kaldi-io.h

// Copyright 2009-2011  Microsoft Corporation;  Jan Silovsky
//                2016  Xiaohui Zhang
#ifndef KALDIFST_CSRC_KALDI_IO_H_
#define KALDIFST_CSRC_KALDI_IO_H_

#include <string>

#include "kaldifst/csrc/log.h"

namespace kaldifst {

class OutputImplBase;  // Forward decl; defined in a .cc file
class InputImplBase;   // Forward decl; defined in a .cc file

// The Output and Input classes handle stream-opening for "extended" filenames
// that include actual files, standard-input/standard-output, pipes, and
// offsets into actual files.  They also handle reading and writing the
// binary-mode headers for Kaldi files, where applicable.  The classes have
// versions of the Open routines that throw and do not throw, depending whether
// the calling code wants to catch the errors or not; there are also versions
// that write (or do not write) the Kaldi binary-mode header that says if it's
// binary mode.  Generally files that contain Kaldi objects will have the header
// on, so we know upon reading them whether they have the header.  So you would
// use the OpenWithHeader routines for these (or the constructor); but other
// types of objects (e.g. FSTs) would have files without a header so you would
// use OpenNoHeader.

// We now document the types of extended filenames that we use.
//
// A "wxfilename"  is an extended filename for writing. It can take three forms:
// (1) Filename: e.g.    "/some/filename", "./a/b/c", "c:\Users\dpovey\My
//                        Documents\\boo"
//          (whatever the actual file-system interprets)
// (2) Standard output:  "" or "-"
// (3) A pipe: e.g. "| gzip -c > /tmp/abc.gz"
//
//
// A "rxfilename" is an extended filename for reading.  It can take four forms:
// (1) An actual filename, whatever the file-system can read, e.g. "/my/file".
// (2) Standard input: "" or "-"
// (3) A pipe: e.g.  "gunzip -c /tmp/abc.gz |"
// (4) An offset into a file, e.g.: "/mnt/blah/data/1.ark:24871"
//   [these are created by the Table and TableWriter classes; I may also write
//    a program that creates them for arbitrary files]
//

// Typical usage:
// ...
// bool binary;
// MyObject.Write(Output(some_filename, binary).Stream(), binary);
//
// ... more extensive example:
// {
//    Output ko(some_filename, binary);
//    MyObject1.Write(ko.Stream(), binary);
//    MyObject2.Write(ko.Stream(), binary);
// }
enum OutputType { kNoOutput, kFileOutput, kStandardOutput, kPipeOutput };

/// ClassifyWxfilename interprets filenames as follows:
///  - kNoOutput: invalid filenames (leading or trailing space, things that look
///     like wspecifiers and rspecifiers or like pipes to read from with leading
///     |.
///  - kFileOutput: Normal filenames
///  - kStandardOutput: The empty string or "-", interpreted as standard output
///  - kPipeOutput: pipes, e.g. "| gzip -c > /tmp/abc.gz"
OutputType ClassifyWxfilename(const std::string &wxfilename);

enum InputType {
  kNoInput,
  kFileInput,
  kStandardInput,
  kOffsetFileInput,
  kPipeInput
};

/// ClassifyRxfilenames interprets filenames for reading as follows:
///  - kNoInput: invalid filenames (leading or trailing space, things that
///       look like wspecifiers and rspecifiers or pipes to write to
///       with trailing |.
///  - kFileInput: normal filenames
///  - kStandardInput: the empty string or "-"
///  - kPipeInput: e.g. "gunzip -c /tmp/abc.gz |"
///  - kOffsetFileInput: offsets into files, e.g.  /some/filename:12970
InputType ClassifyRxfilename(const std::string &rxfilename);

class Output {
 public:
  // The normal constructor, provided for convenience.
  // Equivalent to calling with default constructor then Open()
  // with these arguments.
  Output(const std::string &filename, bool binary, bool write_header = true);

  Output() : impl_(NULL) {}

  /// This opens the stream, with the given mode (binary or text).  It returns
  /// true on success and false on failure.  However, it will throw if something
  /// was already open and could not be closed (to avoid this, call Close()
  /// first.  if write_header == true and binary == true, it writes the Kaldi
  /// binary-mode header ('\0' then 'B').  You may call Open even if it is
  /// already open; it will close the existing stream and reopen (however if
  /// closing the old stream failed it will throw).
  bool Open(const std::string &wxfilename, bool binary, bool write_header);

  inline bool IsOpen();  // return true if we have an open stream.  Does not
  // imply stream is good for writing.

  std::ostream &Stream();  // will throw if not open; else returns stream.

  // Close closes the stream. Calling Close is never necessary unless you
  // want to avoid exceptions being thrown.  There are times when calling
  // Close will hurt efficiency (basically, when using offsets into files,
  // and using the same Input object),
  // but most of the time the user won't be doing this directly, it will
  // be done in kaldi-table.{h, cc}, so you don't have to worry about it.
  bool Close();

  // This will throw if stream could not be closed (to check error status,
  // call Close()).
  ~Output();

 private:
  OutputImplBase *impl_;  // non-NULL if open.
  std::string filename_;
  KALDIFST_DISALLOW_COPY_AND_ASSIGN(Output);
};

}  // namespace kaldifst

#endif  // KALDIFST_CSRC_KALDI_IO_H_
