#!/usr/bin/env python3

import graphviz
import k2
from kaldifst.utils import k2_to_openfst

import kaldifst


def main():
    s = """
    0 1 1 0.5
    0 1 2 1.5
    1 2 3 2.5
    1 3 -1 3.0
    2 3 -1 3.5
    3
    """
    sym = k2.SymbolTable.from_str(
        """
        a 1
        b 2
        c 3
    """
    )
    fsa = k2.Fsa.from_str(s, acceptor=True)
    fsa.labels_sym = sym
    fsa.draw("k2-acceptor.svg")

    fst = k2_to_openfst(fsa)

    fst_dot = kaldifst.draw(fst, acceptor=True, portrait=True)
    source = graphviz.Source(fst_dot)
    source.render(outfile="openfst-acceptor.svg")


if __name__ == "__main__":
    main()
