#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)


import kaldifst


#  ( echo "0 0 0 0"; echo "0 0" ) | fstcompile | fstdeterminizestar | fstprint
def test_determinize_star_case_1(use_log=False):
    print(f"---case 1 use_log: {use_log}---")
    s = """
        0 0 0 0
        0 0
    """
    fst = kaldifst.compile(s, acceptor=False)
    kaldifst.determinize_star(fst, use_log=use_log)
    status = kaldifst.determinize_star(fst)
    print(fst)
    print("status", status)


# ( echo "0 0 1 0"; echo "0 0" ) | fstcompile | fstdeterminizestar | fstprint
def test_determinize_star_case_2(use_log=False):
    print(f"---case 2 use_log: {use_log}---")
    s = """
        0 0 1 0
        0 0
    """
    fst = kaldifst.compile(s, acceptor=False)
    kaldifst.determinize_star(fst, use_log=use_log)
    status = kaldifst.determinize_star(fst)
    print(fst)
    print("status", status)


# ( echo "0 0 1 0"; echo "0 1 1 0"; echo "0 0" ) | fstcompile |
# fstdeterminizestar | fstprint
def test_determinize_star_case_3(use_log=False):
    print(f"---case 3 use_log: {use_log}---")
    s = """
        0 0 1 0
        0 1 1 0
        0 0
    """
    fst = kaldifst.compile(s, acceptor=False)
    status = kaldifst.determinize_star(fst, use_log=use_log)
    print(fst)
    print("status", status)


# ( echo "0 0 0 1"; echo "0 0" ) | fstcompile | fstdeterminizestar | fstprint
# Case 4 fails [correctly]:
def test_determinize_star_case_4(use_log=False):
    print(f"---case 4 use_log: {use_log}---")
    s = """
        0 0 0 1
        0 0
    """
    fst = kaldifst.compile(s, acceptor=False)
    status = kaldifst.determinize_star(fst, use_log=use_log)
    print(fst)
    print("status", status)


def main(use_log=False):
    test_determinize_star_case_1(use_log)
    test_determinize_star_case_2(use_log)
    test_determinize_star_case_3(use_log)
    try:
        test_determinize_star_case_4(use_log)
    except RuntimeError as e:
        print(str(e))


if __name__ == "__main__":
    main(use_log=False)
    main(use_log=True)
