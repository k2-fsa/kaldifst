import kaldifst

s1 = """
0 1 1 4
0 1 3 5
0 1 2 3
0 2 5 2
0 2 4 1
1 2 2 3
1 2 3 1
1 2 1 2
2
"""
fst = kaldifst.compile(s=s1, acceptor=False)
fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
print(fst_dot)

import graphviz

source = graphviz.Source(fst_dot)
source.render(outfile="transducer2.svg")

kaldifst.arcsort(fst, sort_type="olabel")
fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
source = graphviz.Source(fst_dot)
source.render(outfile="sorted-transducer-olabel.svg")
