#!/usr/bin/env python3

import graphviz
import k2
from kaldifst.utils import k2_to_openfst

import kaldifst


def main():
    s = """
    0 1 1 1 0.5
    0 1 2 2 1.5
    1 2 3 3 2.5
    1 3 -1 -1 3.0
    2 3 -1 -1 3.5
    3
    """
    labels_sym = k2.SymbolTable.from_str("""
        a 1
        b 2
        c 3
    """)

    aux_labels_sym = k2.SymbolTable.from_str("""
        x 1
        y 2
        z 3
    """)
    fsa = k2.Fsa.from_str(s, acceptor=False)
    fsa.labels_sym = labels_sym
    fsa.aux_labels_sym = aux_labels_sym
    fsa.draw("k2-transducer.svg")

    fst = k2_to_openfst(fsa, olabels='aux_labels')

    fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
    source = graphviz.Source(fst_dot)
    source.render(outfile="openfst-transducer.svg")


if __name__ == "__main__":
    main()
