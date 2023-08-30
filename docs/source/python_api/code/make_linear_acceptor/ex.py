import graphviz

import kaldifst


sym1 = kaldifst.SymbolTable(name="sym1")
sym1.add_symbol("e", 1)
sym1.add_symbol("h", 2)
sym1.add_symbol("l", 3)
sym1.add_symbol("o", 4)


fst = kaldifst.make_linear_acceptor([2, 1, 3, 3, 4])

fst.input_symbols = sym1
fst.output_symbols = sym1

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="acceptor.svg")
