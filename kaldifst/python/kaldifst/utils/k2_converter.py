# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)
"""
Convert graphs between k2 and OpenFst.
"""

from typing import Optional

import k2
import torch
from _kaldifst import StdArc, StdVectorFst


def _k2_acceptor_to_openfst(fsa: k2.Fsa) -> StdVectorFst:
    """Convert an acceptor in k2 format to a one in OpenFst format.

    Args:
      fsa:
        An acceptor in k2 format.
    Returns:
      Return an acceptor in OpenFst format.
    """
    fsa = fsa.to("cpu")
    assert len(fsa.shape) == 2, "FsaVec is not supported"
    # k2 uses `max` in tropical semiring
    # while OpenFst uses `min`, so we need to
    # negate `fsa.scores`.
    weight = (-1 * fsa.scores).tolist()
    row_splits = fsa.arcs.row_splits(1)
    row_ids = fsa.arcs.row_ids(1)

    num_states = row_ids.numel()
    arcs_per_state = (row_splits[1:] - row_splits[:-1]).tolist()

    fst = StdVectorFst()

    fst.reserve_states(num_states)
    for i in range(num_states):
        s = fst.add_state()
        assert s == i
        fst.reserve_arcs(i, arcs_per_state[i])

    for i, (src, dst, label) in enumerate(fsa.arcs.values()[:, :-1]):
        if label == -1:
            label = 0
        arc = StdArc(
            ilabel=label, olabel=label, weight=weight[i], nextstate=dst
        )
        fst.add_arc(src, arc)

    fst.start = 0
    fst.set_final(num_states - 1, 0)

    return fst


def _k2_transducer_to_openfst(fsa: k2.Fsa, olabels: str) -> StdVectorFst:
    """Convert a transducer in k2 format to an FST in OpenFst format

    Args:
      fsa:
        The input FSA.
      olabels:
        It specifies which attribute from `fsa` contains output labels.
        In most cases, `olabels` is `aux_labels`.
    Returns:
      Return a transducer in OpenFst format.
    """
    assert hasattr(fsa, olabels)
    if isinstance(getattr(fsa, olabels), k2.RaggedTensor):
        fsa = k2.expand_ragged_attributes(fsa, ragged_attribute_names=[olabels])

    fsa = fsa.to("cpu")
    assert len(fsa.shape) == 2, "FsaVec is not supported"

    # k2 uses `max` in tropical semiring
    # while OpenFst uses `min`, so we need to
    # negate `fsa.scores`.
    weight = (-1 * fsa.scores).tolist()
    row_splits = fsa.arcs.row_splits(1)
    row_ids = fsa.arcs.row_ids(1)

    output_labels = getattr(fsa, olabels)
    assert isinstance(output_labels, torch.Tensor)
    output_labels = output_labels.tolist()

    num_states = row_ids.numel()
    arcs_per_state = (row_splits[1:] - row_splits[:-1]).tolist()

    fst = StdVectorFst()
    fst.reserve_states(num_states)

    for i in range(num_states):
        s = fst.add_state()
        assert s == i
        fst.reserve_arcs(i, arcs_per_state[i])

    for i, (src, dst, ilabel) in enumerate(fsa.arcs.values()[:, :-1]):
        olabel = output_labels[i]
        if ilabel == -1:
            ilabel = 0
            assert olabel == -1, olabel
            olabel = 0

        arc = StdArc(
            ilabel=ilabel,
            olabel=olabel,
            weight=weight[i],
            nextstate=dst,
        )
        fst.add_arc(src, arc)

    fst.start = 0
    fst.set_final(num_states - 1, 0)

    return fst


def k2_to_openfst(fsa: k2.Fsa, olabels: Optional[str] = None) -> StdVectorFst:
    """Convert an FSA in k2 format to an FSA in OpenFst format.

    Args:
      fsa:
        An FSA from k2.
        Note:
          Both acceptors and transducers are represented as `k2.Fsa`.
      olabels:
        If it is `None`, we assume the input `fsa` is an acceptor.
        If not `None`, we assume `getattr(fsa, olabel)` contains
        the output labels of arcs. In general, it is calles `aux_labels`
        in k2.
    Return:
     Return an FST in OpenFst format.

    TODO: Also attach symbols, if there are any.
    """
    if olabels is None:
        return _k2_acceptor_to_openfst(fsa)
    else:
        return _k2_transducer_to_openfst(fsa, olabels)
