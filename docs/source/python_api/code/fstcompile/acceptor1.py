import kaldifst

s1 = """
0 1 1 1.5
1 2 2 2.5
2 0.3
"""
fsa = kaldifst.compile(s=s1, acceptor=True)
fsa_dot = kaldifst.draw(fsa, acceptor=True, portrait=True)
print(fsa_dot)

import graphviz

source = graphviz.Source(fsa_dot)
source.render(outfile="acceptor1.svg")
