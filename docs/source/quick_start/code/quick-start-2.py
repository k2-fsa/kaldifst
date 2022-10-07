#!/usr/bin/env python3

import kaldifst
import graphviz


def main():
    s = """
        0 1 a x 0.5
        0 1 b y 1.5
        1 2 c z 2.5
        2 3.5
    """
    isym = kaldifst.SymbolTable.from_str(
        """
        a 1
        b 2
        c 3
    """
    )

    osym = kaldifst.SymbolTable.from_str(
        """
        x 1
        y 2
        z 3
    """
    )

    fst = kaldifst.compile(
        s=s,
        acceptor=False,
        isymbols=isym,
        osymbols=osym,
        keep_isymbols=True,
        keep_osymbols=True,
    )
    fst.write("binary-2.fst")

    fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
    source = graphviz.Source(fst_dot)
    source.render(outfile="quick-start-2.svg")


if __name__ == "__main__":
    main()
