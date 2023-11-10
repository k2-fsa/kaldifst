#!/usr/bin/env python3

import graphviz

import kaldifst

fst = kaldifst.Lattice()
s0 = fst.add_state()
s1 = fst.add_state()
s2 = fst.add_state()
s3 = fst.add_state()

fst.start = s0
fst.add_arc(
    state=s0,
    arc=kaldifst.LatticeArc(
        ilabel=1,
        olabel=1,
        weight=kaldifst.LatticeWeight(graph_cost=0.02, acoustic_cost=0.08),
        nextstate=s1,
    ),
)

fst.add_arc(
    state=s0,
    arc=kaldifst.LatticeArc(
        ilabel=2,
        olabel=2,
        weight=kaldifst.LatticeWeight(graph_cost=0.03, acoustic_cost=0.07),
        nextstate=s2,
    ),
)

fst.add_arc(
    state=s1,
    arc=kaldifst.LatticeArc(
        ilabel=3,
        olabel=3,
        weight=kaldifst.LatticeWeight(graph_cost=0.1, acoustic_cost=0.3),
        nextstate=s3,
    ),
)

fst.add_arc(
    state=s1,
    arc=kaldifst.LatticeArc(
        ilabel=4,
        olabel=4,
        weight=kaldifst.LatticeWeight(graph_cost=0.15, acoustic_cost=0.05),
        nextstate=s3,
    ),
)

fst.add_arc(
    state=s2,
    arc=kaldifst.LatticeArc(
        ilabel=3,
        olabel=3,
        weight=kaldifst.LatticeWeight(graph_cost=0.15, acoustic_cost=0.15),
        nextstate=s3,
    ),
)

fst.add_arc(
    state=s2,
    arc=kaldifst.LatticeArc(
        ilabel=4,
        olabel=4,
        weight=kaldifst.LatticeWeight(graph_cost=0.05, acoustic_cost=0.15),
        nextstate=s3,
    ),
)

fst.set_final(state=s3, weight=kaldifst.LatticeWeight.one)


sym1 = kaldifst.SymbolTable(name="sym1")
sym1.add_symbol("eps", 0)
sym1.add_symbol("a", 1)
sym1.add_symbol("b", 2)
sym1.add_symbol("c", 3)
sym1.add_symbol("d", 4)

sym2 = kaldifst.SymbolTable(name="sym2")
sym2.add_symbol("eps", 0)
sym2.add_symbol("A", 1)
sym2.add_symbol("B", 2)
sym2.add_symbol("C", 3)
sym2.add_symbol("D", 4)

fst.input_symbols = sym1
fst.output_symbols = sym2

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
fst_source = graphviz.Source(fst_dot)
fst_source.render(outfile="lattice.svg")

nbest_list = kaldifst.lattice_to_nbest(fst, n=3)
for b in nbest_list:
    b.input_symbols = fst.input_symbols
    b.output_symbols = fst.output_symbols

nbest_list_0_dot = kaldifst.draw(nbest_list[0], acceptor=True, portrait=True)
nbest_list_0_source = graphviz.Source(nbest_list_0_dot)
nbest_list_0_source.render(outfile="lattice-3best-0.svg")

nbest_list_1_dot = kaldifst.draw(nbest_list[1], acceptor=True, portrait=True)
nbest_list_1_source = graphviz.Source(nbest_list_1_dot)
nbest_list_1_source.render(outfile="lattice-3best-1.svg")

nbest_list_2_dot = kaldifst.draw(nbest_list[2], acceptor=True, portrait=True)
nbest_list_2_source = graphviz.Source(nbest_list_2_dot)
nbest_list_2_source.render(outfile="lattice-3best-2.svg")
