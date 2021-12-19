#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


def test_float_weight():
    w = kaldifst.FloatWeight(2.0)
    assert w.value == 2.0
    assert w == kaldifst.FloatWeight(2.0)
    assert str(w) == "2"
    # Note: You cannot change w.value once it is set


def test_tropical_weight():
    w = kaldifst.TropicalWeight(1)
    assert w.value == 1

    # Note: You cannot change w.value once it is set
    w2 = kaldifst.TropicalWeight.zero
    assert w2 == float("inf")

    w3 = kaldifst.TropicalWeight.one
    assert w3 == 0.0

    w4 = kaldifst.TropicalWeight.no_weight
    assert str(w4) == "nan"


def test_tropical_weight_operations():
    w1 = kaldifst.TropicalWeight(2)
    w2 = kaldifst.TropicalWeight(3)

    w3 = kaldifst.plus(w1, w2)  # return the minimum, min(w1, w2)
    assert w3 == 2

    w4 = kaldifst.times(w1, w2)  # return the sum: w1 + w2
    assert w4 == 5

    w5 = kaldifst.divide(w1, w2)  # return thed difference: w1 - w2
    assert w5 == -1


def main():
    test_float_weight()
    test_tropical_weight()
    test_tropical_weight_operations()


if __name__ == "__main__":
    main()
