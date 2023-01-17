// kaldifst/csrc/kaldi-semaphore.h

// Copyright 2012  Karel Vesely (Brno University of Technology)
//           2017  Dogan Can (University of Southern California)

// See ../../COPYING for clarification regarding multiple authors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// THIS CODE IS PROVIDED *AS IS* BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION ANY IMPLIED
// WARRANTIES OR CONDITIONS OF TITLE, FITNESS FOR A PARTICULAR PURPOSE,
// MERCHANTABLITY OR NON-INFRINGEMENT.
// See the Apache 2 License for the specific language governing permissions and
// limitations under the License.

#ifndef KALDIFST_CSRC_KALDI_SEMAPHORE_H_
#define KALDIFST_CSRC_KALDI_SEMAPHORE_H_

#include <condition_variable>  // NOLINT
#include <mutex>               // NOLINT

#include "kaldifst/csrc/log.h"

namespace kaldifst {

class Semaphore {
 public:
  explicit Semaphore(int32_t count = 0);

  ~Semaphore();

  bool TryWait();  ///< Returns true if Wait() goes through
  void Wait();     ///< decrease the counter
  void Signal();   ///< increase the counter

 private:
  int32_t count_;  ///< the semaphore counter, 0 means block on Wait()

  std::mutex mutex_;
  std::condition_variable condition_variable_;
  KALDIFST_DISALLOW_COPY_AND_ASSIGN(Semaphore);
};

}  // namespace kaldifst

#endif  // KALDIFST_CSRC_KALDI_SEMAPHORE_H_
