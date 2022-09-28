import kaldifst

s1 = """
0 1 a A 1.5
1 2 b B 2.5
2 0.3
"""
isym = kaldifst.SymbolTable(name="An arbitrary name")
isym.add_symbol("a", 1)
isym.add_symbol("b", 2)

osym = kaldifst.SymbolTable(name="Another arbitrary name")
osym.add_symbol("A", 1)
osym.add_symbol("B", 2)

fst = kaldifst.compile(
    s=s1,
    acceptor=False,
    isymbols=isym,
    osymbols=osym,
    keep_isymbols=False,
    keep_osymbols=False,
)
fst.input_symbols = isym
fst.output_symbols = osym
fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
print(fst_dot)

import graphviz

source = graphviz.Source(fst_dot)
source.render(outfile="transducer2.svg")
