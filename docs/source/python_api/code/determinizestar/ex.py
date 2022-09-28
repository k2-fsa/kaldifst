import graphviz

import kaldifst

s = """
0 1 a p 1
0 2 a q 2
0 1 eps q 3
1 3 c r 5
1 3 c r 4
2 3 d s 6
3 0
"""

sym1 = kaldifst.SymbolTable(name="sym1")
sym1.add_symbol("eps", 0)
sym1.add_symbol("a", 1)
sym1.add_symbol("c", 2)
sym1.add_symbol("d", 3)

sym2 = kaldifst.SymbolTable(name="sym2")
sym2.add_symbol("eps", 0)
sym2.add_symbol("p", 1)
sym2.add_symbol("q", 2)
sym2.add_symbol("r", 3)
sym2.add_symbol("s", 4)

fst = kaldifst.compile(s=s, acceptor=False, isymbols=sym1, osymbols=sym2)
fst.input_symbols = sym1
fst.output_symbols = sym2

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="transducer.svg")

kaldifst.determinize_star(fst)

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="transducer2.svg")
