#!/usr/bin/env python3

import graphviz

import kaldifst

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

isym = kaldifst.SymbolTable.from_str(
    """
        a 1
        b 2
        c 3
        d 4
        f 5
        g 6
"""
)

fst = kaldifst.compile(
    s,
    acceptor=True,
    isymbols=isym,
    keep_isymbols=True,
)

fst_dot = kaldifst.draw(fst, acceptor=True, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="acceptor.svg")

kaldifst.minimize(fst)

fst_dot = kaldifst.draw(fst, acceptor=True, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="acceptor-minimized.svg")
