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

fst.set_final(state=s3, weight=kaldifst.LatticeWeight.one)

print(fst)
# The above statement gives the following output
"""
0      1      1      2      0.1,0.2
0      2      3      4      0.3,0.4
1      3      5      6      0.5,0.6
2      3      6      8      0.7,0.8
3
"""

# Now we can draw it
fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
print(fst_dot)
# You can use
# https://dreampuf.github.io/GraphvizOnline
# to visualize it and share the link to others

# Alternatively, you can use graphviz to visualize it
source = graphviz.Source(fst_dot)
source.render(outfile="lattice.svg")
