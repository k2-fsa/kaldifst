import kaldifst

s1 = """
0 1 1
0 1 3
0 1 2
0 2 5
0 2 4
1 2 2
1 2 3
1 2 1
2
"""
fsa = kaldifst.compile(s=s1, acceptor=True)
fsa_dot = kaldifst.draw(fsa, acceptor=True, portrait=True)
print(fsa_dot)

import graphviz

source = graphviz.Source(fsa_dot)
source.render(outfile="acceptor.svg")

kaldifst.arcsort(fsa)
fsa_dot = kaldifst.draw(fsa, acceptor=True, portrait=True)

source2 = graphviz.Source(fsa_dot)
source2.render(outfile="sorted.svg")
