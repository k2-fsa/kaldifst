#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


def test_compose():
    isym = kaldifst.SymbolTable()
    isym.add_symbol("a", 1)
    isym.add_symbol("c", 2)

    osym = kaldifst.SymbolTable()
    osym.add_symbol("q", 1)
    osym.add_symbol("r", 2)
    osym.add_symbol("s", 3)

    s = """
    0 1 a q 1
    0 2 a r 2.5
    1 1 c s 1
    1 0
    2 2.5
    """

    fst1 = kaldifst.compile(s, acceptor=False, isymbols=isym, osymbols=osym)
    s1 = kaldifst.draw(
        fst1, acceptor=False, isymbols=isym, osymbols=osym, portrait=True
    )
    print(s1)
    # see https://git.io/JStSD
    # for how s1 looks like

    # Now for fst2

    osym2 = kaldifst.SymbolTable()
    osym2.add_symbol("f", 1)
    osym2.add_symbol("h", 2)
    osym2.add_symbol("g", 3)
    osym2.add_symbol("j", 4)
    s2 = """
        0 1 q f 1
        0 2 r h 3
        1 2 s g 2.5
        2 2 s j 1.5
        2 2
    """
    fst2 = kaldifst.compile(s2, acceptor=False, isymbols=osym, osymbols=osym2)
    s2 = kaldifst.draw(
        fst2, acceptor=False, isymbols=osym, osymbols=osym2, portrait=True
    )
    print(s2)

    # see
    # https://git.io/JStHU
    # for how s2 looks like

    fst3 = kaldifst.compose(
        fst1,
        fst2,
        match_side="left",
        compose_filter="sequence",
        connect=True,
    )
    s3 = kaldifst.draw(
        fst3,
        acceptor=False,
        isymbols=isym,
        osymbols=osym2,
        portrait=True,
    )
    print(s3)
    # see
    # https://git.io/JStQ5
    # for what s3 looks like

    # see also
    # https://www.openfst.org/twiki/bin/view/FST/ComposeDoc


def main():
    test_compose()


if __name__ == "__main__":
    main()
