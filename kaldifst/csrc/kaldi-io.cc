// kaldifst/csrc/kaldi-io.cc
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/util/kaldi-io.cc

// Copyright 2009-2011  Microsoft Corporation;  Jan Silovsky
//                2016  Xiaohui Zhang
#include "kaldifst/csrc/kaldi-io.h"

#include <string.h>

#include "kaldifst/csrc/kaldi-pipebuf.h"
#include "kaldifst/csrc/kaldi-table.h"
#include "kaldifst/csrc/log.h"
#include "kaldifst/csrc/parse-options.h"

#define MapOsPath(x) x

namespace kaldifst {

std::string PrintableWxfilename(const std::string &wxfilename) {
  if (wxfilename == "" || wxfilename == "-") {
    return "standard output";
  } else {
    // If this call to Escape later causes compilation issues,
    // just replace it with "return wxfilename"; it's only a
    // pretty-printing issue.
    return ParseOptions::Escape(wxfilename);
  }
}

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

#ifndef _MSC_VER  // on VS, we don't need this type.
// could replace basic_pipebuf<char> with stdio_filebuf<char> on some platforms.
// Would mean we could use less of our own code.
typedef basic_pipebuf<char> PipebufType;
#endif

class OutputImplBase {
 public:
  // Open will open it as a file (no header), and return true
  // on success.  It cannot be called on an already open stream.
  virtual bool Open(const std::string &filename, bool binary) = 0;
  virtual std::ostream &Stream() = 0;
  virtual bool Close() = 0;
  virtual ~OutputImplBase() {}
};

class FileOutputImpl : public OutputImplBase {
 public:
  virtual bool Open(const std::string &filename, bool binary) {
    if (os_.is_open())
      KALDIFST_ERR << "FileOutputImpl::Open(), "
                   << "open called on already open file.";
    filename_ = filename;
    os_.open(MapOsPath(filename_).c_str(),
             binary ? std::ios_base::out | std::ios_base::binary
                    : std::ios_base::out);
    return os_.is_open();
  }

  virtual std::ostream &Stream() {
    if (!os_.is_open())
      KALDIFST_ERR << "FileOutputImpl::Stream(), file is not open.";
    // I believe this error can only arise from coding error.
    return os_;
  }

  virtual bool Close() {
    if (!os_.is_open())
      KALDIFST_ERR << "FileOutputImpl::Close(), file is not open.";
    // I believe this error can only arise from coding error.
    os_.close();
    return !(os_.fail());
  }
  virtual ~FileOutputImpl() {
    if (os_.is_open()) {
      os_.close();
      if (os_.fail()) KALDIFST_ERR << "Error closing output file " << filename_;
    }
  }

 private:
  std::string filename_;
  std::ofstream os_;
};

class StandardOutputImpl : public OutputImplBase {
 public:
  StandardOutputImpl() : is_open_(false) {}

  virtual bool Open(const std::string &filename, bool binary) {
    if (is_open_)
      KALDIFST_ERR << "StandardOutputImpl::Open(), "
                      "open called on already open file.";
#ifdef _MSC_VER
    _setmode(_fileno(stdout), binary ? _O_BINARY : _O_TEXT);
#endif
    is_open_ = std::cout.good();
    return is_open_;
  }

  virtual std::ostream &Stream() {
    if (!is_open_)
      KALDIFST_ERR << "StandardOutputImpl::Stream(), object not initialized.";
    // I believe this error can only arise from coding error.
    return std::cout;
  }

  virtual bool Close() {
    if (!is_open_)
      KALDIFST_ERR << "StandardOutputImpl::Close(), file is not open.";
    is_open_ = false;
    std::cout << std::flush;
    return !(std::cout.fail());
  }
  virtual ~StandardOutputImpl() {
    if (is_open_) {
      std::cout << std::flush;
      if (std::cout.fail()) KALDIFST_ERR << "Error writing to standard output";
    }
  }

 private:
  bool is_open_;
};

class PipeOutputImpl : public OutputImplBase {
 public:
  PipeOutputImpl() : f_(NULL), os_(NULL) {}

  virtual bool Open(const std::string &wxfilename, bool binary) {
    filename_ = wxfilename;
    KALDIFST_ASSERT(f_ == NULL);  // Make sure closed.
    KALDIFST_ASSERT(wxfilename.length() != 0 &&
                    wxfilename[0] == '|');  // should
    // start with '|'
    std::string cmd_name(wxfilename, 1);
#if defined(_MSC_VER) || defined(__CYGWIN__)
    f_ = popen(cmd_name.c_str(), (binary ? "wb" : "w"));
#else
    f_ = popen(cmd_name.c_str(), "w");
#endif
    if (!f_) {  // Failure.
      KALDIFST_WARN << "Failed opening pipe for writing, command is: "
                    << cmd_name << ", errno is " << strerror(errno);
      return false;
    } else {
#ifndef _MSC_VER
      fb_ = new PipebufType(f_,  // Using this constructor won't make the
                                 // destructor try to close the stream when
                                 // we're done.
                            (binary ? std::ios_base::out | std::ios_base::binary
                                    : std::ios_base::out));
      KALDIFST_ASSERT(fb_ != NULL);  // or would be alloc error.
      os_ = new std::ostream(fb_);
#else
      os_ = new std::ofstream(f_);
#endif
      return os_->good();
    }
  }

  virtual std::ostream &Stream() {
    if (os_ == NULL)
      KALDIFST_ERR << "PipeOutputImpl::Stream(),"
                      " object not initialized.";
    // I believe this error can only arise from coding error.
    return *os_;
  }

  virtual bool Close() {
    if (os_ == NULL)
      KALDIFST_ERR << "PipeOutputImpl::Close(), file is not open.";
    bool ok = true;
    os_->flush();
    if (os_->fail()) ok = false;
    delete os_;
    os_ = NULL;
    int status;
#ifdef _MSC_VER
    status = _pclose(f_);
#else
    status = pclose(f_);
#endif
    if (status)
      KALDIFST_WARN << "Pipe " << filename_ << " had nonzero return status "
                    << status;
    f_ = NULL;
#ifndef _MSC_VER
    delete fb_;
    fb_ = NULL;
#endif
    return ok;
  }
  virtual ~PipeOutputImpl() {
    if (os_) {
      if (!Close())
        KALDIFST_ERR << "Error writing to pipe "
                     << PrintableWxfilename(filename_);
    }
  }

 private:
  std::string filename_;
  FILE *f_;
#ifndef _MSC_VER
  PipebufType *fb_;
#endif
  std::ostream *os_;
};

Output::Output(const std::string &wxfilename, bool binary, bool write_header)
    : impl_(NULL) {
  if (!Open(wxfilename, binary, write_header)) {
    if (impl_) {
      delete impl_;
      impl_ = NULL;
    }
    KALDIFST_ERR << "Error opening output stream "
                 << PrintableWxfilename(wxfilename);
  }
}

}  // namespace kaldifst
