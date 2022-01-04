#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


# see also https://www.openfst.org/twiki/bin/view/FST/InvertDoc
def test_invert():
    isym = kaldifst.SymbolTable()
    isym.add_symbol("a", 1)
    isym.add_symbol("b", 2)
    isym.add_symbol("c", 3)
    isym.add_symbol("d", 4)
    isym.add_symbol("f", 5)

    osym = kaldifst.SymbolTable()
    osym.add_symbol("u", 1)
    osym.add_symbol("v", 2)
    osym.add_symbol("w", 3)
    osym.add_symbol("x", 4)
    osym.add_symbol("y", 5)

    s = """
        0 1 a y 1
        0 1 b x 3
        1 1 d v 7
        1 2 c w 5
        2 3 f u 9
        3 2
    """

    fst = kaldifst.compile(s, acceptor=False, isymbols=isym, osymbols=osym)
    s1 = kaldifst.draw(
        fst, acceptor=False, isymbols=isym, osymbols=osym, portrait=True
    )
    print(s1)  # see https://git.io/JSP8I

    kaldifst.invert(fst)  # in-place

    s2 = kaldifst.draw(
        fst, acceptor=False, isymbols=osym, osymbols=isym, portrait=True
    )
    print(s2)  # see https://git.io/JSPRL


def main():
    test_invert()


if __name__ == "__main__":
    main()
