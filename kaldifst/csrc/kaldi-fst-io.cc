// kaldifst/csrc/kaldi-fst-io.cc
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/kaldi-fst-io.cc

// Copyright 2009-2011  Microsoft Corporation
//           2012-2015  Johns Hopkins University (Author: Daniel Povey)
//                2013  Guoguo Chen

#include "kaldifst/csrc/kaldi-fst-io.h"

#include "fst/fst-decl.h"
#include "fst/fst.h"
#include "fst/fstlib.h"
#include "kaldifst/csrc/kaldi-io.h"
#include "kaldifst/csrc/log.h"

namespace fst {

VectorFst<StdArc> *ReadFstKaldi(std::string rxfilename) {
  if (rxfilename == "") rxfilename = "-";  // interpret "" as stdin,
  // for compatibility with OpenFst conventions.
  kaldifst::Input ki(rxfilename);
  FstHeader hdr;
  if (!hdr.Read(ki.Stream(), rxfilename))
    KALDIFST_ERR << "Reading FST: error reading FST header from "
                 << kaldifst::PrintableRxfilename(rxfilename);
  FstReadOptions ropts("<unspecified>", &hdr);
  VectorFst<StdArc> *fst = VectorFst<StdArc>::Read(ki.Stream(), ropts);
  if (!fst)
    KALDIFST_ERR << "Could not read fst from "
                 << kaldifst::PrintableRxfilename(rxfilename);
  return fst;
}

// Register const fst to load it automatically. Other types like
// olabel_lookahead or ngram or compact_fst should be registered
// through OpenFst registration API.
static FstRegisterer<VectorFst<StdArc>> VectorFst_StdArc_registerer;
static FstRegisterer<ConstFst<StdArc>> ConstFst_StdArc_registerer;

Fst<StdArc> *ReadFstKaldiGeneric(std::string rxfilename, bool throw_on_err) {
  if (rxfilename == "") rxfilename = "-";  // interpret "" as stdin,
  // for compatibility with OpenFst conventions.
  kaldifst::Input ki(rxfilename);
  FstHeader hdr;
  // Read FstHeader which contains the type of FST
  if (!hdr.Read(ki.Stream(), rxfilename)) {
    if (throw_on_err) {
      KALDIFST_ERR << "Reading FST: error reading FST header from "
                   << kaldifst::PrintableRxfilename(rxfilename);
    } else {
      KALDIFST_WARN << "We fail to read FST header from "
                    << kaldifst::PrintableRxfilename(rxfilename)
                    << ". A NULL pointer is returned.";
      return NULL;
    }
  }
  // Check the type of Arc
  if (hdr.ArcType() != StdArc::Type()) {
    if (throw_on_err) {
      KALDIFST_ERR << "FST with arc type " << hdr.ArcType()
                   << " is not supported.";
    } else {
      KALDIFST_WARN << "Fst with arc type" << hdr.ArcType()
                    << " is not supported. A NULL pointer is returned.";
      return NULL;
    }
  }
  // Read the FST
  FstReadOptions ropts("<unspecified>", &hdr);
  Fst<StdArc> *fst = Fst<StdArc>::Read(ki.Stream(), ropts);
  if (!fst) {
    if (throw_on_err) {
      KALDIFST_ERR << "Could not read fst from "
                   << kaldifst::PrintableRxfilename(rxfilename);
    } else {
      KALDIFST_WARN << "Could not read fst from "
                    << kaldifst::PrintableRxfilename(rxfilename)
                    << ". A NULL pointer is returned.";
      return NULL;
    }
  }
  return fst;
}

VectorFst<StdArc> *CastOrConvertToVectorFst(Fst<StdArc> *fst) {
  // This version currently supports ConstFst<StdArc> or VectorFst<StdArc>
  std::string real_type = fst->Type();
  KALDIFST_ASSERT(real_type == "vector" || real_type == "const");
  if (real_type == "vector") {
    return dynamic_cast<VectorFst<StdArc> *>(fst);
  } else {
    // As the 'fst' can't cast to VectorFst, we create a new
    // VectorFst<StdArc> initialized by 'fst', and delete 'fst'.
    VectorFst<StdArc> *new_fst = new VectorFst<StdArc>(*fst);
    delete fst;
    return new_fst;
  }
}

void ReadFstKaldi(std::string rxfilename, fst::StdVectorFst *ofst) {
  fst::StdVectorFst *fst = ReadFstKaldi(rxfilename);
  *ofst = *fst;
  delete fst;
}

void WriteFstKaldi(const VectorFst<StdArc> &fst, std::string wxfilename) {
  if (wxfilename == "") wxfilename = "-";  // interpret "" as stdout,
  // for compatibility with OpenFst conventions.
  bool write_binary = true, write_header = false;
  kaldifst::Output ko(wxfilename, write_binary, write_header);
  FstWriteOptions wopts(kaldifst::PrintableWxfilename(wxfilename));
  fst.Write(ko.Stream(), wopts);
}

VectorFst<fst::StdArc> *ReadAndPrepareLmFst(const std::string &rxfilename) {
  // ReadFstKaldi() will die with exception on failure.
  VectorFst<fst::StdArc> *ans = ReadFstKaldi(rxfilename);
  if (ans->Properties(fst::kAcceptor, true) == 0) {
    // If it's not already an acceptor, project on the output, i.e. copy olabels
    // to ilabels.  Generally the G.fst's on disk will have the disambiguation
    // symbol #0 on the input symbols of the backoff arc, and projection will
    // replace them with epsilons which is what is on the output symbols of
    // those arcs.
    Project(ans, fst::PROJECT_OUTPUT);
  }
  if (ans->Properties(fst::kILabelSorted, true) == 0) {
    // Make sure LM is sorted on ilabel.
    ILabelCompare<fst::StdArc> ilabel_comp;
    ArcSort(ans, ilabel_comp);
  }
  return ans;
}

}  // namespace fst
