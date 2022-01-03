#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


def test_arc_sort():
    s = """
    0 1 1 4
    0 1 3 5
    0 1 2 3
    0 2 5 2
    0 2 4 1
    1 2 2 3
    1 2 3 1
    1 2 1 2
    2
    """
    fst = kaldifst.compile(s, acceptor=False)

    assert fst.is_ilabel_sorted is False
    assert fst.is_olabel_sorted is False

    s1 = kaldifst.draw(fst, acceptor=False, portrait=True)
    print(s1)  # see https://git.io/JSVap

    kaldifst.arcsort(fst, sort_type="ilabel")

    assert fst.is_ilabel_sorted is True
    assert fst.is_olabel_sorted is False

    s2 = kaldifst.draw(fst, acceptor=False, portrait=True)
    print(s2)  # see https://git.io/JSVwK

    kaldifst.arcsort(fst, sort_type="olabel")

    assert fst.is_ilabel_sorted is False
    assert fst.is_olabel_sorted is True

    s3 = kaldifst.draw(fst, acceptor=False, portrait=True)
    print(s3)  # see https://git.io/JSVr8

    # see also https://www.openfst.org/twiki/bin/view/FST/ArcSortDoc


def main():
    test_arc_sort()


if __name__ == "__main__":
    main()
