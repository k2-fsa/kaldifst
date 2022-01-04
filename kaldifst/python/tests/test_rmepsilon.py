#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


# see also https://www.openfst.org/twiki/bin/view/FST/RmEpsilonDoc
def test_rmepsilon():
    isym = kaldifst.SymbolTable()
    isym.add_symbol("<eps>", 0)
    isym.add_symbol("a", 1)

    osym = kaldifst.SymbolTable()
    osym.add_symbol("<eps>", 0)
    osym.add_symbol("p", 1)

    s = """
        0 1 <eps> <eps> 1
        1 2 a <eps> 2
        1 2 <eps> p 3
        1 2 <eps> <eps> 4
        2 2 <eps> <eps> 5
        2 3 <eps> <eps> 6
        3 7
    """
    fst = kaldifst.compile(s, acceptor=False, isymbols=isym, osymbols=osym)
    s1 = kaldifst.draw(
        fst, acceptor=False, isymbols=isym, osymbols=osym, portrait=True
    )
    print(s1)  # see https://git.io/JSXx0

    kaldifst.rmepsilon(fst)
    s2 = kaldifst.draw(
        fst, acceptor=False, isymbols=isym, osymbols=osym, portrait=True
    )
    print(s2)  # see https://git.io/JSXp0


def main():
    test_rmepsilon()


if __name__ == "__main__":
    main()
