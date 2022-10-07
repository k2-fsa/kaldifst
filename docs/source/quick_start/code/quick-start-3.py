#!/usr/bin/env python3

import graphviz

import kaldifst


def main():
    fst = kaldifst.StdVectorFst.read("binary.fst")
    for state in kaldifst.StateIterator(fst):
        for arc in kaldifst.ArcIterator(fst, state):

            # Note: We can change the attribute of the arc if we want
            if arc.weight == 0.5:
                arc.weight = 0.6
            elif arc.weight == 1.5:
                arc.weight = 1.8
            elif arc.weight == 2.5:
                arc.weight = 1.25

            print(state, arc)

    fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
    source = graphviz.Source(fst_dot)
    source.render(outfile="quick-start-3.svg")


if __name__ == "__main__":
    main()
"""
Output of this file:

0 (ilabel: 1, olabel: 1, weight: 0.6, nextstate: 1)
0 (ilabel: 2, olabel: 2, weight: 1.8, nextstate: 1)
1 (ilabel: 3, olabel: 3, weight: 1.25, nextstate: 2)
"""
