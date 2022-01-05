#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


# see also https://www.openfst.org/twiki/bin/view/FST/ReverseDoc
def test_reverse():
    isym = kaldifst.SymbolTable()
    isym.add_symbol("eps", 0)
    isym.add_symbol("a", 1)
    isym.add_symbol("b", 2)
    isym.add_symbol("c", 3)
    isym.add_symbol("d", 4)
    isym.add_symbol("f", 5)

    osym = isym
    s = """
        0 0 a a 2
        0 1 a a 1
        1 3
        1 2 b b 3
        1 2 c c 4
        2 2 d d 5
        2 3 d d 6
        3 3 f f 2
        3 2
    """

    fst = kaldifst.compile(s, acceptor=False, isymbols=isym, osymbols=osym)
    s1 = kaldifst.draw(
        fst, acceptor=False, isymbols=isym, osymbols=osym, portrait=True
    )
    print(s1)  # see https://git.io/JSDdH

    fst2 = kaldifst.reverse(fst)

    s2 = kaldifst.draw(
        fst2, acceptor=False, isymbols=osym, osymbols=isym, portrait=True
    )
    print(s2)  # see https://git.io/JSDdh


def main():
    test_reverse()


if __name__ == "__main__":
    main()
