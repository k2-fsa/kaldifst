#!/usr/bin/env python3

import graphviz

import kaldifst

s = """
0 0 a 2
0 1 a 1
1 3
1 2 b 3
1 2 b 4
2 2 d 5
2 3 d 6
3 3 f 2
3 2
"""
isym = kaldifst.SymbolTable.from_str(
    """
        eps 0
        a 1
        b 2
        c 3
        d 4
        f 5
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
fst_source.render(outfile="acceptor-before-reverse-2.svg")

fst = kaldifst.reverse(fst, require_superinitial=False)

fst_dot = kaldifst.draw(fst, acceptor=True, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="acceptor-after-reverse-2.svg")
