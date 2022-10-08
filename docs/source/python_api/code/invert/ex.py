#!/usr/bin/env python3

import graphviz

import kaldifst

s = """
0 1 a y 1
0 1 b x 3
1 1 d v 7
1 2 c w 5
2 3 f u 9
3 2
"""
isym = kaldifst.SymbolTable.from_str(
    """
        a 1
        b 2
        c 3
        d 4
        f 5
"""
)

osym = kaldifst.SymbolTable.from_str(
    """
        x 1
        y 2
        u 3
        w 4
        v 5
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
fst_source.render(outfile="before-invert.svg")

kaldifst.invert(fst)

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="after-invert.svg")
