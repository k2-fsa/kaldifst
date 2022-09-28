import graphviz

import kaldifst

s1 = """
0 1 a q 1
0 2 a r 2.5
1 1 c s 1
1 0
2 2.5
"""

sym1 = kaldifst.SymbolTable(name="sym1")
sym1.add_symbol("a", 1)
sym1.add_symbol("c", 2)

sym2 = kaldifst.SymbolTable(name="sym2")
sym2.add_symbol("q", 1)
sym2.add_symbol("r", 2)
sym2.add_symbol("s", 3)

fst = kaldifst.compile(s=s1, acceptor=False, isymbols=sym1, osymbols=sym2)
fst.input_symbols = sym1
fst.output_symbols = sym2

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
with open("fst_dot.txt", "w") as f:
    f.write(fst_dot)

fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="fst.svg")
