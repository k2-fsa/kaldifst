#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


def test_std_arc():
    arc = kaldifst.StdArc(ilabel=1, olabel=2, weight=1.5, nextstate=3)
    assert arc.ilabel == 1
    assert arc.olabel == 2
    assert arc.weight == 1.5
    assert arc.nextstate == 3
    assert arc.type() == "standard"

    assert isinstance(arc.weight, kaldifst.TropicalWeight)

    arc.weight = 2.0
    assert arc.weight == 2.0

    arc.weight = kaldifst.TropicalWeight(3.0)
    assert arc.weight == 3.0


if __name__ == "__main__":
    test_std_arc()
