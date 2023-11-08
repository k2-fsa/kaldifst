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
        olabel=2,
        weight=kaldifst.LatticeWeight(graph_cost=0.1, acoustic_cost=0.2),
        nextstate=s1,
    ),
)

fst.add_arc(
    state=s0,
    arc=kaldifst.LatticeArc(
        ilabel=3,
        olabel=4,
        weight=kaldifst.LatticeWeight(graph_cost=0.3, acoustic_cost=0.4),
        nextstate=s2,
    ),
)

fst.add_arc(
    state=s1,
    arc=kaldifst.LatticeArc(
        ilabel=5,
        olabel=6,
        weight=kaldifst.LatticeWeight(graph_cost=0.5, acoustic_cost=0.6),
        nextstate=s3,
    ),
)

fst.add_arc(
    state=s2,
    arc=kaldifst.LatticeArc(
        ilabel=6,
        olabel=8,
        weight=kaldifst.LatticeWeight(graph_cost=0.7, acoustic_cost=0.8),
        nextstate=s3,
    ),
)

fst.set_final(
    state=s3, weight=kaldifst.LatticeWeight(graph_cost=0.2, acoustic_cost=0.5)
)

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
source = graphviz.Source(fst_dot)
source.render(outfile="before-scale-2.svg")

scale = [[0.1, 1], [0.5, 10]]

kaldifst.scale_lattice(scale, fst)

fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
source = graphviz.Source(fst_dot)
source.render(outfile="after-scale-2.svg")
