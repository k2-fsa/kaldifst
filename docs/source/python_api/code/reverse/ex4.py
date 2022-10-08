#!/usr/bin/env python3

import graphviz

import kaldifst

s = """
0 1 a a
1 2 b b
1 3 c c
2 4 eps eps
3 4 eps eps
4
"""
isym = kaldifst.SymbolTable.from_str(
    """
        eps 0
        a 1
        b 2
        c 3
"""
)

fst = kaldifst.compile(
    s,
    acceptor=False,
    isymbols=isym,
    osymbols=isym,
    keep_isymbols=True,
    keep_osymbols=True,
)

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="transducer-before-reverse-2.svg")

fst = kaldifst.reverse(fst, require_superinitial=False)

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="transducer-after-reverse-2.svg")
