#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)

from kaldifst import StateIterator
from kaldifst import ArcIterator
from kaldifst import StdVectorFst
from kaldifst import StdArc
from kaldifst import info
from kaldifst import draw

import os


def test_constructor():
    fst = StdVectorFst()
    s0 = fst.add_state()
    fst.start = s0

    s1 = fst.add_state()

    arc = StdArc(ilabel=1, olabel=2, weight=0.5, nextstate=s1)
    fst.add_arc(s0, arc)
    fst.set_final(s1, weight=0.0)
    print(fst)
    print("-.-" * 10)
    print(fst.to_str())
    print("-.-" * 10)
    print(fst.to_str(fst_field_separator=","))
    print("-.-" * 10)
    print(fst.to_str(show_weight_one=True))


def test_iterator():
    fst = StdVectorFst()
    s0 = fst.add_state()
    fst.start = s0

    s1 = fst.add_state()

    fst.add_arc(s0, StdArc(ilabel=1, olabel=1, weight=0.5, nextstate=s1))
    fst.add_arc(s0, StdArc(ilabel=2, olabel=2, weight=1.5, nextstate=s1))

    fst.add_arc(s1, StdArc(ilabel=3, olabel=3, weight=2.5, nextstate=2))

    s2 = fst.add_state()
    assert s2 == 2

    fst.set_final(s2, weight=3.5)

    # test iterator
    for state in StateIterator(fst):
        for arc in ArcIterator(fst, state):
            print(state, arc)
    fst.write("a.fst")

    # You can use:
    #  fstprint a.fst
    os.remove("a.fst")

    import kaldifst

    info(fst)
    print()
    dot_str = draw(fst, fontsize=20)
    print(dot_str)
    print(help(draw))


def main():
    test_constructor()
    test_iterator()


if __name__ == "__main__":
    main()
