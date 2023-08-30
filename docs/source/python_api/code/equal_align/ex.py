import graphviz

import kaldifst

s1 = """
0 0 e E 0.3
0 1 a A 1
0 1 b B 2.5
1 2 <eps> <eps> 0.3
1 2 <eps> <eps> 0.4
1 1 f F 0.03
2 2 g G 0.8
2 3 h H 0.12
3
"""

sym1 = kaldifst.SymbolTable(name="sym1")
sym1.add_symbol("<eps>", 0)
sym1.add_symbol("a", 1)
sym1.add_symbol("b", 2)
sym1.add_symbol("c", 3)
sym1.add_symbol("d", 4)
sym1.add_symbol("e", 5)
sym1.add_symbol("f", 6)
sym1.add_symbol("g", 7)
sym1.add_symbol("h", 8)

sym2 = kaldifst.SymbolTable(name="sym2")
sym2.add_symbol("<eps>", 0)
sym2.add_symbol("A", 1)
sym2.add_symbol("B", 2)
sym2.add_symbol("C", 3)
sym2.add_symbol("D", 4)
sym2.add_symbol("E", 5)
sym2.add_symbol("F", 6)
sym2.add_symbol("G", 7)
sym2.add_symbol("H", 8)

fst = kaldifst.compile(s=s1, acceptor=False, isymbols=sym1, osymbols=sym2)

fst.input_symbols = sym1
fst.output_symbols = sym2

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="input.svg")

succeeded, first = kaldifst.equal_align(
    ifst=fst, length=4, rand_seed=3, num_retries=10
)
assert succeeded is True
first.input_symbols = sym1
first.output_symbols = sym1

first_dot = kaldifst.draw(first, acceptor=False, portrait=True)
first_source = graphviz.Source(first_dot)
first_source.render(outfile="first.svg")

succeeded, second = kaldifst.equal_align(
    ifst=fst, length=5, rand_seed=10, num_retries=10
)
assert succeeded is True
second.input_symbols = sym1
second.output_symbols = sym2

second_dot = kaldifst.draw(second, acceptor=False, portrait=True)
second_source = graphviz.Source(second_dot)
second_source.render(outfile="second.svg")
