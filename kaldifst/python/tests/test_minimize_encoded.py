#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


def test_minimize_encoded_1():
    s = """
        0 0 0 0
        0 0
    """
    fst = kaldifst.compile(s, acceptor=False)
    print(fst)

    kaldifst.minimize_encoded(fst)
    print(fst)


def test_minimize_encoded_2():
    s = """
        0 1 0 0
        0 2 0 0
        1 0
        2 0
    """
    fst = kaldifst.compile(s, acceptor=False)
    print(fst)

    kaldifst.minimize_encoded(fst)
    print(fst)


def main():
    test_minimize_encoded_1()
    test_minimize_encoded_2()


if __name__ == "__main__":
    main()
