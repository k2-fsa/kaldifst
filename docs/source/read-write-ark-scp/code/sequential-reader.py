#!/usr/bin/env python3
import kaldifst


def test_sequential_reader():
    rspecifier = f"scp:lat.scp"
    with kaldifst.SequentialVectorFstReader(rspecifier) as ki:
        for key, value in ki:
            assert isinstance(value, kaldifst.StdVectorFst)
            print("key", key)
            print("value\n", str(value))


def main():
    test_sequential_reader()


if __name__ == "__main__":
    main()
