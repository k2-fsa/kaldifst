// kaldifst/python/csrc/context-fst.cc
//
// Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifst/python/csrc/context-fst.h"

#include <utility>
#include <vector>

#include "kaldifst/csrc/context-fst.h"

static constexpr const char *kComposeContextDoc = R"doc(
compose_context composes efficiently with a context fst
that it generates.

Without ``disambig_syms`` specified, it
assumes that all input symbols of ``ifst`` are phones.
It adds the subsequential symbol itself (it does not
appear in the output so doesn't need to be specified by the user).
the ``disambig_syms`` is a list of disambiguation symbols on the LHS
of ``ifst``.  The symbols on the LHS of ``out.fst`` are indexes into
the ``ilabels.list`` file, which is a kaldi-format file containing a
``vector<vector<int32>>``, which specifies what the labels mean in
terms of windows of symbols.

Args:
  disambig_syms:
    List of disambiguation symbols, e.g. the integer ids of #0, #1, #2 ... in
    the phones.txt.
  context_width:
    Size of context window, e.g. 3 for triphone.
  central_position:
    Central position in phonetic context window (zero-based index), e.g. 1 for
    triphone.
  ifst:
    The FST we are composing with C (e.g. LG.fst),
  project_ifst:
    This is intended only to be set to true in the program
    'fstmakecontextfst'... if true, it will project on the input after adding
    the subsequential loop to 'ifst', which allows us to reconstruct the
    context fst C.fst.

Returns:
  Return a tuple containing (ofst, ilabels), where:

    - ofst: The resulting fst
    - ilabels, a List[List[int]]
)doc";

namespace kaldifst {

static void PybindComposeContext(py::module *m) {
  m->def(
      "compose_context",
      [](const std::vector<int32_t> &disambig_syms, int32_t context_width,
         int32_t central_position, fst::VectorFst<fst::StdArc> *ifst,
         bool project_ifst) -> std::pair<fst::VectorFst<fst::StdArc>,
                                         std::vector<std::vector<int32_t>>> {
        std::vector<std::vector<int32_t>> ilabels;
        fst::VectorFst<fst::StdArc> composed_fst;

        auto lhs = ifst->Copy();
        fst::ComposeContext(disambig_syms, context_width, central_position, lhs,
                            &composed_fst, &ilabels);
        delete lhs;

        return std::make_pair(composed_fst, ilabels);
      },
      py::arg("disambig_syms"), py::arg("context_width"),
      py::arg("central_position"), py::arg("ifst"),
      py::arg("project_ifst") = false, kComposeContextDoc);
}

void PybindContextFst(py::module *m) { PybindComposeContext(m); }

}  // namespace kaldifst
