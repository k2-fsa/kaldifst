#!/usr/bin/env python3
import kaldifst


def test_random_access_reader():
    rspecifier = f"scp:lat.scp"
    with kaldifst.RandomAccessVectorFstReader(rspecifier) as ki:
        assert "lat1" in ki
        assert "lat2" in ki
        lat1 = ki["lat1"]
        print("lat1", lat1)

        lat2 = ki["lat2"]
        print("lat2", lat2)


def main():
    test_random_access_reader()


if __name__ == "__main__":
    main()
