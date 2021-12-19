#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)

import os

import kaldifst


def test_symbol_table():
    assert kaldifst.SymbolTable.kNoSymbol == -1
    sym = kaldifst.SymbolTable()
    sym.add_symbol("a", 1)
    sym.add_symbol("b", 2)
    c_id = sym.add_symbol("c")  # return the ID of "c"
    assert c_id == 3
    sym.name = "test"
    assert sym.name == "test"

    assert 1 in sym
    assert 2 in sym
    assert 3 in sym
    assert "a" in sym
    assert "b" in sym
    assert "c" in sym

    assert sym.find(1) == "a"
    assert sym.find(2) == "b"
    assert sym.find(3) == "c"
    assert sym.find(100) == ""  # return an empty string

    assert sym.find("a") == 1
    assert sym.find("b") == 2
    assert sym.find("c") == 3
    assert sym.find("d") == kaldifst.SymbolTable.kNoSymbol == -1

    assert sym.num_symbols() == 3
    assert sym.available_key() == 4  # return the highest key + 1

    sym.remove_symbol(2)  # can only remove an ID
    assert "b" not in sym

    assert sym.num_symbols() == 2
    assert sym.available_key() == 4  # return the highest key + 1

    sym.remove_symbol(3)
    assert sym.num_symbols() == 1

    hello_id = sym.add_symbol("hello")
    assert sym.find(hello_id) == "hello"
    assert sym.find("hello") == hello_id

    # write binary format
    sym.write("sym.bin")
    sym2 = kaldifst.SymbolTable.read("sym.bin")
    assert sym.check_sum == sym2.check_sum
    assert sym.labeled_check_sum == sym2.labeled_check_sum
    assert sym2.name == "test"

    # write text format
    sym.write_text("sym.txt")
    sym2 = kaldifst.SymbolTable.read_text("sym.txt")
    assert sym.check_sum == sym2.check_sum
    assert sym.labeled_check_sum == sym2.labeled_check_sum
    assert sym2.name == "sym.txt"

    os.remove("sym.bin")
    os.remove("sym.txt")


if __name__ == "__main__":
    test_symbol_table()
