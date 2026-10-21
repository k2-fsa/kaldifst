#!/usr/bin/env python3
#
# Copyright (c)  2026  Xiaomi Corporation (author: Fangjun Kuang)
#
# Smoke test for the built wheels. Run by build-wheels-win-arm64.yaml and
# build-wheels-android.yaml after installing a freshly built wheel.
#
# It builds and walks a real FST so that a wheel which cannot load its
# extension module (missing shared library, wrong architecture, wrong
# platform tag) fails here rather than on a user's machine.
#
# It uses no third-party dependencies -- only the kaldifst package itself --
# so it runs anywhere the wheel installs.

import platform
import sys

import kaldifst
from kaldifst import ArcIterator, StateIterator, StdArc, StdVectorFst, info


def build_fst() -> StdVectorFst:
    # s0 --1/1:0.5--> s1 --3/3:2.5--> s2 (final 3.5)
    fst = StdVectorFst()
    s0 = fst.add_state()
    fst.start = s0

    s1 = fst.add_state()
    fst.add_arc(s0, StdArc(ilabel=1, olabel=1, weight=0.5, nextstate=s1))

    s2 = fst.add_state()
    fst.add_arc(s1, StdArc(ilabel=3, olabel=3, weight=2.5, nextstate=s2))
    fst.set_final(s2, weight=3.5)
    return fst


def main() -> int:
    print("kaldifst:", getattr(kaldifst, "__version__", "<none>"), kaldifst.__file__)
    print("python:", sys.version.replace("\n", " "))
    print("platform:", platform.platform())
    print("machine:", platform.machine())

    fst = build_fst()

    num_states = sum(1 for _ in StateIterator(fst))
    num_arcs = sum(1 for state in StateIterator(fst) for _ in ArcIterator(fst, state))
    print("num_states:", num_states)
    print("num_arcs:", num_arcs)

    assert num_states == 3, f"expected 3 states, got {num_states}"
    assert num_arcs == 2, f"expected 2 arcs, got {num_arcs}"
    assert fst.start == 0, f"expected start state 0, got {fst.start}"

    # Exercise a real algorithm, not just construction. info() prints to
    # stdout and returns None, so success here means it did not raise.
    info(fst)

    # Round-trip through the text format as a second, independent check that
    # the extension is genuinely working.
    text = fst.to_str()
    print("to_str:")
    print(text)
    assert "1" in text and "3" in text, "unexpected to_str() output"

    print("SMOKE TEST PASSED")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
