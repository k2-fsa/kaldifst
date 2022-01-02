#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)

import kaldifst
from kaldifst import (
    ArcIterator,
    StateIterator,
    StdArc,
    StdConstFst,
    StdVectorFst,
)


def test_constructor():
    vector_fst = StdVectorFst()
    vector_fst.add_state()
    vector_fst.add_arc(0, StdArc(1, 2, 0.5, nextstate=1))
    vector_fst.add_arc(0, StdArc(1, 2, 0.5, nextstate=2))

    vector_fst.add_state()
    vector_fst.add_state()
    vector_fst.add_arc(1, StdArc(1, 3, 1.5, nextstate=2))
    vector_fst.start = 0
    vector_fst.set_final(2, 0.3)

    const_fst = StdConstFst(vector_fst)
    print(const_fst)
    for state in StateIterator(const_fst):
        for arc in ArcIterator(const_fst, state):
            print(state, arc)


def test_compile():
    s = """
    0 1 1 2 0.5
    1 2 3 1 0.3
    2 1.2
    """
    fst = kaldifst.compile(s, fst_type="const")
    assert isinstance(fst, kaldifst.StdConstFst)
    print(help(fst))

    fst = kaldifst.compile(s, fst_type="vector")
    assert isinstance(fst, kaldifst.StdVectorFst)


def main():
    test_constructor()
    test_compile()


if __name__ == "__main__":
    main()
