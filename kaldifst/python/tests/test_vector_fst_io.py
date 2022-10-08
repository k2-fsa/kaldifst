#!/usr/bin/env python3

# Copyright      2022  Xiaomi Corporation (authors: Fangjun Kuang)

import kaldifst

base = "lat"
wspecifier = f"ark,scp:{base}.ark,{base}.scp"
rspecifier = f"scp:{base}.scp"


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
    with kaldifst.VectorFstWriter(wspecifier) as ko:
        ko.write("lat1", fsa)
        ko.write("lat2", fst)


def test_sequential_reader():
    with kaldifst.SequentialVectorFstReader(rspecifier) as ki:
        for key, value in ki:
            assert isinstance(value, kaldifst.StdVectorFst)
            print("key", key)
            print("value\n", str(value))


def test_random_access_reader():
    with kaldifst.RandomAccessVectorFstReader(rspecifier) as ki:
        assert "lat1" in ki
        assert "lat2" in ki
        lat1 = ki["lat1"]
        print("lat1", lat1)

        lat2 = ki["lat2"]
        print("lat2", lat2)


def main():
    test_writer()
    test_sequential_reader()
    test_random_access_reader()


if __name__ == "__main__":
    main()
