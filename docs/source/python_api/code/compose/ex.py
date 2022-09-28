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

a = kaldifst.compile(s=s1, acceptor=False, isymbols=sym1, osymbols=sym2)
a.input_symbols = sym1
a.output_symbols = sym2

s2 = """
0 1 q f 1
0 2 r h 3
1 2 s g 2.5
2 2 s j 1.5
2 2
"""

sym3 = kaldifst.SymbolTable(name="sym3")
sym3.add_symbol("f", 1)
sym3.add_symbol("g", 2)
sym3.add_symbol("h", 3)
sym3.add_symbol("j", 4)

b = kaldifst.compile(s=s2, acceptor=False, isymbols=sym2, osymbols=sym3)
b.input_symbols = sym2
b.output_symbols = sym3

a_dot = kaldifst.draw(a, acceptor=False, portrait=True)
a_source = graphviz.Source(a_dot)
a_source.render(outfile="a.svg")

b_dot = kaldifst.draw(b, acceptor=False, portrait=True)
b_source = graphviz.Source(b_dot)
b_source.render(outfile="b.svg")

# sort b by ilabel. It is sorted in-place
kaldifst.arcsort(b, sort_type="ilabel")

c = kaldifst.compose(a, b)
c_dot = kaldifst.draw(c, acceptor=False, portrait=True)
c_source = graphviz.Source(c_dot)
c_source.render(outfile="c.svg")
