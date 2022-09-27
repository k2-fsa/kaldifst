import kaldifst

s1 = """
0 1 a 1.5
1 2 b 2.5
1 0.8
2 0.3
"""
isym = kaldifst.SymbolTable(name="An arbitrary name")
isym.add_symbol("a", 1)
isym.add_symbol("b", 2)
fsa = kaldifst.compile(s=s1, acceptor=True, isymbols=isym, keep_isymbols=False)
fsa.input_symbols = isym
fsa_dot = kaldifst.draw(fsa, portrait=True)
print(fsa_dot)

import graphviz

source = graphviz.Source(fsa_dot)
source.render(outfile="acceptor2.svg")
