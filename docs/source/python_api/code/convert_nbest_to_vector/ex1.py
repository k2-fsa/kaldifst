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

nbest_3 = kaldifst.shortest_path(a, n=3)
nbest_3_dot = kaldifst.draw(nbest_3, acceptor=True, portrait=True)
nbest_3_source = graphviz.Source(nbest_3_dot)
nbest_3_source.render(outfile="vector-fst-3best.svg")

nbest_list = kaldifst.convert_nbest_to_vector(nbest_3)
for b in nbest_list:
    b.input_symbols = a.input_symbols
    b.output_symbols = a.output_symbols

nbest_list_0_dot = kaldifst.draw(nbest_list[0], acceptor=True, portrait=True)
nbest_list_0_source = graphviz.Source(nbest_list_0_dot)
nbest_list_0_source.render(outfile="vector-fst-3best-0.svg")

nbest_list_1_dot = kaldifst.draw(nbest_list[1], acceptor=True, portrait=True)
nbest_list_1_source = graphviz.Source(nbest_list_1_dot)
nbest_list_1_source.render(outfile="vector-fst-3best-1.svg")

nbest_list_2_dot = kaldifst.draw(nbest_list[2], acceptor=True, portrait=True)
nbest_list_2_source = graphviz.Source(nbest_list_2_dot)
nbest_list_2_source.render(outfile="vector-fst-3best-2.svg")
