import kaldifst

s1 = """
0 1 1 10 1.5
1 2 2 20 2.5
2 0.3
"""
fst = kaldifst.compile(s=s1, acceptor=False)
fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
print(fst_dot)

import graphviz

source = graphviz.Source(fst_dot)
source.render(outfile="transducer1.svg")
