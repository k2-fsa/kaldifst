#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)

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


def main():
    test_constructor()


if __name__ == "__main__":
    main()
