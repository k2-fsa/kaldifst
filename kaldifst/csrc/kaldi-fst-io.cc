// kaldifst/csrc/kaldi-fst-io.cc
//
// This file is copied/modified from
// https://github.com/kaldi-asr/kaldi/blob/master/src/fstext/kaldi-fst-io.cc

// Copyright 2009-2011  Microsoft Corporation
//           2012-2015  Johns Hopkins University (Author: Daniel Povey)
//                2013  Guoguo Chen

#include "kaldifst/csrc/kaldi-fst-io.h"

#include "fst/fst.h"
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

}  // namespace fst
