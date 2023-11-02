#!/usr/bin/env python3

import graphviz

import kaldifst

s1 = """
0 1 a 0.1
0 2 b 0.1
1 3 c 0.4
1 3 d 0.2
2 3 c 0.3
2 3 d 0.2
3 0
"""

sym1 = kaldifst.SymbolTable(name="sym1")
sym1.add_symbol("eps", 0)
sym1.add_symbol("a", 1)
sym1.add_symbol("b", 2)
sym1.add_symbol("c", 3)
sym1.add_symbol("d", 4)

a = kaldifst.compile(s=s1, acceptor=True, isymbols=sym1)
a.input_symbols = sym1

a_dot = kaldifst.draw(a, acceptor=True, portrait=True)
a_source = graphviz.Source(a_dot)
a_source.render(outfile="vector-fst.svg")

nbest_1 = kaldifst.shortest_path(a, n=1)
nbest_1_dot = kaldifst.draw(nbest_1, acceptor=True, portrait=True)
nbest_1_source = graphviz.Source(nbest_1_dot)
nbest_1_source.render(outfile="vector-fst-1best.svg")

nbest_2 = kaldifst.shortest_path(a, n=2)
nbest_2_dot = kaldifst.draw(nbest_2, acceptor=True, portrait=True)
nbest_2_source = graphviz.Source(nbest_2_dot)
nbest_2_source.render(outfile="vector-fst-2best.svg")

nbest_3 = kaldifst.shortest_path(a, n=3)
nbest_3_dot = kaldifst.draw(nbest_3, acceptor=True, portrait=True)
nbest_3_source = graphviz.Source(nbest_3_dot)
nbest_3_source.render(outfile="vector-fst-3best.svg")
