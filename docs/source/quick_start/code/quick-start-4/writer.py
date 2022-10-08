#!/usr/bin/env python3
import kaldifst


def generate_acceptor():
    s = """
    0 1 a 0.5
    1 2 b 0.8
    2 0.25
    """
    isym = kaldifst.SymbolTable.from_str(
        """
      a 1
      b 2
    """
    )
    fsa = kaldifst.compile(
        s,
        acceptor=True,
        isymbols=isym,
        keep_isymbols=True,
    )
    return fsa


def generate_transducer():
    s = """
    0 1 a A 0.5
    1 2 b B 0.8
    2 0.25
    """
    isym = kaldifst.SymbolTable.from_str(
        """
      a 1
      b 2
    """
    )

    osym = kaldifst.SymbolTable.from_str(
        """
      A 1
      B 2
    """
    )
    fst = kaldifst.compile(
        s,
        acceptor=False,
        isymbols=isym,
        osymbols=osym,
        keep_isymbols=True,
        keep_osymbols=True,
    )
    return fst


def test_writer():
    fsa = generate_acceptor()
    fst = generate_transducer()

    wspecifier = f"ark,scp:lat.ark,lat.scp"
    with kaldifst.VectorFstWriter(wspecifier) as ko:
        ko.write("lat1", fsa)
        ko.write("lat2", fst)


def main():
    test_writer()


if __name__ == "__main__":
    main()
