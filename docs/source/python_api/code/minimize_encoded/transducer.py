#!/usr/bin/env python3

import graphviz

import kaldifst

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

osym = kaldifst.SymbolTable.from_str(
    """
        a 1
        b 2
        c 3
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

kaldifst.minimize_encoded(fst)

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="transducer-minimize-encoded.svg")
