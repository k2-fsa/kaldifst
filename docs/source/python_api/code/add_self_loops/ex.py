import graphviz

import kaldifst

s = """
0 1 a p
1
1 2 b q
2 3 c r
3 4 f t
3 0 d s
5 0 f t
"""


sym1 = kaldifst.SymbolTable(name="sym1")
sym1.add_symbol("a", 1)
sym1.add_symbol("b", 2)
sym1.add_symbol("c", 3)
sym1.add_symbol("d", 4)
sym1.add_symbol("f", 5)
sym1.add_symbol("#0", 6)
sym1.add_symbol("#1", 7)

sym2 = kaldifst.SymbolTable(name="sym2")
sym2.add_symbol("p", 1)
sym2.add_symbol("q", 2)
sym2.add_symbol("r", 3)
sym2.add_symbol("s", 4)
sym2.add_symbol("t", 5)
sym2.add_symbol("#0", 6)
sym2.add_symbol("#1", 7)

fst = kaldifst.compile(s=s, acceptor=False, isymbols=sym1, osymbols=sym2)

fst.input_symbols = sym1
fst.output_symbols = sym2

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="fst.svg")

kaldifst.add_self_loops(fst, isyms=[6, 7], osyms=[6, 7])

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="fst-add-self-loops.svg")
