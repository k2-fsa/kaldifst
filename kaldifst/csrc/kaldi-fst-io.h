// kaldifst/csrc/kaldi-fst-io.h
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/kaldi-fst-io.h

// Copyright 2009-2011  Microsoft Corporation
//           2012-2015  Johns Hopkins University (Author: Daniel Povey)
//                2013  Guoguo Chen

#ifndef KALDIFST_CSRC_KALDI_FST_IO_H_
#define KALDIFST_CSRC_KALDI_FST_IO_H_

#include <string>

#include "fst/fst.h"
#include "fst/vector-fst.h"
#include "kaldifst/csrc/log.h"

// Some functions for writing Fsts.
// I/O for FSTs is a bit of a mess, and not very well integrated with Kaldi's
// generic I/O mechanisms, because we want files containing just FSTs to
// be readable by OpenFST's native binaries, which is not compatible
// with the normal \0B header that identifies Kaldi files as containing
// binary data.
// So use the functions here with your eyes open, and with caution!
namespace fst {

// Read a binary FST using Kaldi I/O mechanisms (pipes, etc.)
// On error returns NULL. Only supports VectorFst and exists
// mainly for backward code compabibility.
VectorFst<StdArc> *ReadFstKaldi(std::string rxfilename);

// Read a binary FST using Kaldi I/O mechanisms (pipes, etc.)
// If it can't read the FST, if throw_on_err == true it throws using
// KALDIFST_ERR; otherwise it prints a warning and returns. Note:this doesn't
// support the text-mode option that we generally like to support. This version
// currently supports ConstFst<StdArc> or VectorFst<StdArc> (const-fst can give
// better performance for decoding). Other types could be also loaded if
// registered inside OpenFst.
Fst<StdArc> *ReadFstKaldiGeneric(std::string rxfilename,
                                 bool throw_on_err = true);

// This function attempts to dynamic_cast the pointer 'fst' (which will likely
// have been returned by ReadFstGeneric()), to the more derived
// type VectorFst<StdArc>. If this succeeds, it returns the same pointer;
// if it fails, it converts the FST type (by creating a new VectorFst<stdArc>
// initialized by 'fst'), prints a warning, and deletes 'fst'.
VectorFst<StdArc> *CastOrConvertToVectorFst(Fst<StdArc> *fst);

// Version of ReadFstKaldi() that writes to a pointer.  Assumes
// the FST is binary with no binary marker.  Crashes on error.
void ReadFstKaldi(std::string rxfilename, VectorFst<StdArc> *ofst);

// Write an FST using Kaldi I/O mechanisms (pipes, etc.)
// On error, throws using KALDIFST_ERR.  For use only in code in fstbin/,
// as it doesn't support the text-mode option.
void WriteFstKaldi(const VectorFst<StdArc> &fst, std::string wxfilename);

// This is a more general Kaldi-type-IO mechanism of writing FSTs to
// streams, supporting binary or text-mode writing.  (note: we just
// write the integers, symbol tables are not supported).
// On error, throws using KALDIFST_ERR.
template <class Arc>
void WriteFstKaldi(std::ostream &os, bool binary, const VectorFst<Arc> &fst);

// A generic Kaldi-type-IO mechanism of reading FSTs from streams,
// supporting binary or text-mode reading/writing.
template <class Arc>
void ReadFstKaldi(std::istream &is, bool binary, VectorFst<Arc> *fst);

// Read an FST file for LM (G.fst) and make it an acceptor,
// and make sure it is sorted on labels
VectorFst<StdArc> *ReadAndPrepareLmFst(std::string rxfilename);

// This is a Holder class with T = VectorFst<Arc>, that meets the requirements
// of a Holder class as described in ../util/kaldi-holder.h. This enables us to
// read/write collections of FSTs indexed by strings, using the Table concept (
// see ../util/kaldi-table.h).
// Originally it was only templated on T = VectorFst<StdArc>, but as the keyword
// spotting stuff introduced more types of FSTs, we made it also templated on
// the arc.
template <class Arc>
class VectorFstTplHolder {
 public:
  typedef VectorFst<Arc> T;

  VectorFstTplHolder() : t_(NULL) {}

  static bool Write(std::ostream &os, bool binary, const T &t);

  void Copy(const T &t) {  // copies it into the holder.
    Clear();
    t_ = new T(t);
  }

  // Reads into the holder.
  bool Read(std::istream &is);

  // It's potentially a binary format, so must read in binary mode (linefeed
  // translation will corrupt the file.  We don't know till we open the file if
  // it's really binary, so we need to read in binary mode to be on the safe
  // side.  Extra linefeeds won't matter, the text-mode reading code ignores
  // them.
  static bool IsReadInBinary() { return true; }

  T &Value() {
    // code error if !t_.
    if (!t_) KALDIFST_ERR << "VectorFstTplHolder::Value() called wrongly.";
    return *t_;
  }

  void Clear() {
    if (t_) {
      delete t_;
      t_ = NULL;
    }
  }

  void Swap(VectorFstTplHolder<Arc> *other) { std::swap(t_, other->t_); }

  bool ExtractRange(const VectorFstTplHolder<Arc> &other,
                    const std::string &range) {
    KALDIFST_ERR << "ExtractRange is not defined for this type of holder.";
    return false;
  }

  ~VectorFstTplHolder() { Clear(); }
  // No destructor.  Assignment and
  // copy constructor take their default implementations.
 private:
  KALDIFST_DISALLOW_COPY_AND_ASSIGN(VectorFstTplHolder);
  T *t_;
};

// Now make the original VectorFstHolder as the typedef of
// VectorFstHolder<StdArc>.
typedef VectorFstTplHolder<StdArc> VectorFstHolder;

}  // namespace fst

#include "kaldifst/csrc/kaldi-fst-io-inl.h"

#endif  // KALDIFST_CSRC_KALDI_FST_IO_H_
