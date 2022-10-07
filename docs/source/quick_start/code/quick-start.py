#!/usr/bin/env python3

import kaldifst
import graphviz


def main():
    # A vector FST is a general mutable FST
    fst = kaldifst.StdVectorFst()

    # Adds state 0 to the initially empty FST and make it the start state.
    s0 = fst.add_state()  # 1st state will be state 0 (returned by add_state)
    assert s0 == 0
    fst.start = 0  # set the start state to 0

    # Adds two arcs exiting state 0.
    # Arc constructor args: ilabel, olabel, weight, dest state ID.
    fst.add_arc(
        state=0,
        arc=kaldifst.StdArc(ilabel=1, olabel=1, weight=0.5, nextstate=1),
    )

    fst.add_arc(
        state=0,
        arc=kaldifst.StdArc(ilabel=2, olabel=2, weight=1.5, nextstate=1),
    )

    # Adds state 1 and its arc.
    fst.add_state()
    fst.add_arc(
        state=1,
        arc=kaldifst.StdArc(ilabel=3, olabel=3, weight=2.5, nextstate=2),
    )

    # Adds state 2 and set its final weight.
    fst.add_state()
    fst.set_final(state=2, weight=3.5)  # 1st arg is state ID, 2nd arg weight

    # Add an input symbol table
    isym = kaldifst.SymbolTable()
    isym.add_symbol(symbol="a", key=1)
    isym.add_symbol("b", 2)
    isym.add_symbol("c", 3)
    fst.input_symbols = isym

    # Add an output symbol table
    osym = kaldifst.SymbolTable()
    osym.add_symbol("x", 1)
    osym.add_symbol("y", 2)
    osym.add_symbol("z", 3)
    fst.output_symbols = osym

    # We can save this FST to a file with
    fst.write(filename="binary.fst")

    # We can read it back
    fst2 = kaldifst.StdVectorFst.read("binary.fst")
    print(fst2)
    print(fst2.to_str())

    # Get a dot format of the FST for visualization
    fst_dot = kaldifst.draw(fst, acceptor=False, portrait=True)
    print(fst_dot)
    # You can use
    # https://dreampuf.github.io/GraphvizOnline
    # to visualize it and share the link to others

    # Alternatively, you can use graphviz to visualize it
    source = graphviz.Source(fst_dot)
    source.render(outfile="quick-start.svg")
    # Done. quick-start.svg is the figure we showed at the start of this section


if __name__ == "__main__":
    main()
