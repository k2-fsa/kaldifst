#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


# see also https://www.openfst.org/twiki/bin/view/FST/DeterminizeDoc
def test_determinize():
    isym = kaldifst.SymbolTable()
    isym.add_symbol("<eps>", 0)
    isym.add_symbol("a", 1)
    isym.add_symbol("c", 2)
    isym.add_symbol("d", 3)

    osym = kaldifst.SymbolTable()
    osym.add_symbol("<eps>", 0)
    osym.add_symbol("p", 1)
    osym.add_symbol("q", 2)
    osym.add_symbol("r", 3)
    osym.add_symbol("s", 4)

    s = """
        0 1 a p 1
        0 2 a q 2
        1 3 c r 5
        1 3 c r 4
        2 3 d s 6
        3 0
    """

    fst = kaldifst.compile(s, acceptor=False, isymbols=isym, osymbols=osym)
    s1 = kaldifst.draw(
        fst, acceptor=False, isymbols=isym, osymbols=osym, portrait=True
    )
    print(s1)  # see https://git.io/JSPvH

    det_fst = kaldifst.determinize(fst)
    s2 = kaldifst.draw(
        det_fst, acceptor=False, isymbols=isym, osymbols=osym, portrait=True
    )
    print(s2)  # see https://git.io/JSPTF


def main():
    test_determinize()


if __name__ == "__main__":
    main()
