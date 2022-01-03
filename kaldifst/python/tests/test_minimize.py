#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


# see also https://www.openfst.org/twiki/bin/view/FST/MinimizeDoc
def test_minimize_acceptor():
    sym = kaldifst.SymbolTable()
    sym.add_symbol("a", 1)
    sym.add_symbol("b", 2)
    sym.add_symbol("c", 3)
    sym.add_symbol("d", 4)
    sym.add_symbol("f", 5)
    sym.add_symbol("g", 6)

    s = """
        0 1 a 2
        0 1 b 2
        0 1 c 3
        0 2 d 3
        0 2 f 1
        1 3 f 3
        1 3 g 2
        3 1
        2 4 f 5
        2 4 g 4
        4 3
    """
    fst = kaldifst.compile(s, acceptor=True, isymbols=sym)
    s1 = kaldifst.draw(fst, acceptor=True, isymbols=sym, portrait=True)
    print(s1)  # see https://git.io/JSwRL

    kaldifst.minimize(fst)
    s2 = kaldifst.draw(fst, acceptor=True, isymbols=sym, portrait=True)
    print(s2)  # see https://git.io/JSwRY


# see also https://www.openfst.org/twiki/bin/view/FST/MinimizeDoc
def test_minimize_transducer():
    isym = kaldifst.SymbolTable()
    isym.add_symbol("a", 1)
    isym.add_symbol("b", 2)
    isym.add_symbol("c", 3)
    isym.add_symbol("d", 4)
    isym.add_symbol("f", 5)
    isym.add_symbol("g", 6)

    osym = kaldifst.SymbolTable()
    osym.add_symbol("a", 1)
    osym.add_symbol("b", 2)
    osym.add_symbol("c", 3)

    s = """
        0 1 a a 2
        0 1 b a 2
        0 1 c a 3
        0 2 d a 3
        0 2 f a 1
        1 3 f b 3
        1 3 g b 2
        3 1
        2 4 f c 5
        2 4 g c 4
        4 3
    """
    fst = kaldifst.compile(s, acceptor=False, isymbols=isym, osymbols=osym)
    s1 = kaldifst.draw(
        fst, acceptor=False, isymbols=isym, osymbols=osym, portrait=True
    )
    print(s1)  # see https://git.io/JSw0u

    kaldifst.minimize(fst)
    s2 = kaldifst.draw(
        fst, acceptor=False, isymbols=isym, osymbols=osym, portrait=True
    )
    print(s2)  # see https://git.io/JSwga


def main():
    test_minimize_acceptor()
    test_minimize_transducer()


if __name__ == "__main__":
    main()
