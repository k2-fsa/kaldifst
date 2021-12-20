#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


def test_transducer():
    s = """
    0 1 1 2 0.5
    0 1 2 3 1.5
    1 2 3 5 0.2
    2 0.5
    """
    fst = kaldifst.compile(s, fst_type="vector", arc_type="standard")
    assert isinstance(fst, kaldifst.StdVectorFst)


def test_acceptor():
    s = """
    0 1 a 0.5
    1 0.5
    """
    isym = kaldifst.SymbolTable(name="my_input_symbol")
    isym.add_symbol("a", 1)
    fst = kaldifst.compile(s, acceptor=True, isymbols=isym, keep_isymbols=False)
    print(fst.to_str(is_acceptor=True))
    assert fst.input_symbols is None

    fst.input_symbols = isym

    print(fst.to_str(is_acceptor=True))


def main():
    test_transducer()
    test_acceptor()


if __name__ == "__main__":
    main()
