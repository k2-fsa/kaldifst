#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)
import k2
from kaldifst.utils import k2_to_openfst

import kaldifst


def test_k2_acceptor_to_openfst():
    print("--- test_k2_acceptor_to_openfst ---")
    s = """
        0 1 1 0.1
        0 2 2 0.2
        1 3 -1 0.3
        2 3 -1 0.4
        3
    """
    fsa = k2.Fsa.from_str(s, acceptor=True)
    fst = k2_to_openfst(fsa)

    s = kaldifst.draw(fst, acceptor=False, portrait=True)
    print(s)


def test_k2_transducer_to_openfst():
    print("--- test_k2_transdcuer_to_openfst ---")
    s = """
        0 1 1 11 0.1
        0 2 2 22 0.2
        1 3 -1 -1 0.3
        2 3 -1 -1 0.4
        3
    """
    fsa = k2.Fsa.from_str(s, acceptor=False)
    fst = k2_to_openfst(fsa, olabels="aux_labels")

    s = kaldifst.draw(fst, acceptor=False, portrait=True)
    print(s)


def test_k2_transducer_ragged_attribute_to_openfst():
    print("--- test_k2_transducer_ragged_attribute_to_openfst ---")
    s = """
        0 1 0.1
        0 2 0.2
        1 3 -1 0.3
        2 3 -1 0.4
        3
    """
    fsa = k2.Fsa.from_str(s, acceptor=True)
    fsa.aux_labels = k2.RaggedTensor([[1, 2], [10, 20, 30], [], [4, 5]])
    fst = k2_to_openfst(fsa, olabels="aux_labels")

    s = kaldifst.draw(fst, acceptor=False, portrait=True)
    print(s)


def main():
    test_k2_acceptor_to_openfst()
    test_k2_transducer_to_openfst()
    test_k2_transducer_ragged_attribute_to_openfst()


if __name__ == "__main__":
    main()
