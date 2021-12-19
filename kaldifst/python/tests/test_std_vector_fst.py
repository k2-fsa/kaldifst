#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


def test_std_vector_fst():
    fst = kaldifst.StdVectorFst()
    s0 = fst.add_state()
    fst.start = s0

    s1 = fst.add_state()

    arc = kaldifst.StdArc(ilabel=1, olabel=2, weight=0.5, nextstate=s1)
    fst.add_arc(s0, arc)
    fst.set_final(s1, weight=0.0)
    print(fst)
    print("-.-" * 10)
    print(fst.to_str())
    print("-.-" * 10)
    print(fst.to_str(fst_field_separator=","))
    print("-.-" * 10)
    print(fst.to_str(show_weight_one=True))


def main():
    test_std_vector_fst()


if __name__ == "__main__":
    main()
