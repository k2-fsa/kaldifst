#!/usr/bin/env python3

import graphviz

import kaldifst

s = """
0 1 eps eps 1
1 2 a eps 2
1 2 eps p 3
1 2 eps eps 4
2 2 eps eps 5
2 3 eps eps 6
3 7
"""
isym = kaldifst.SymbolTable.from_str(
    """
        eps 0
        a 1
"""
)

osym = kaldifst.SymbolTable.from_str(
    """
        eps 0
        p 1
"""
)

fst = kaldifst.compile(
    s,
    acceptor=False,
    isymbols=isym,
    osymbols=osym,
    keep_isymbols=True,
    keep_osymbols=True,
)


fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="transducer.svg")

kaldifst.rmepsilon(fst)

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="transducer-after-rmepsilon.svg")
