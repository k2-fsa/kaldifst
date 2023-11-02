#!/usr/bin/env python3

import kaldifst

m = kaldifst.lattice_scale(lmwt=0.1, acwt=10.0)
assert m == [[0.1, 0.0], [0.0, 10.0]], m
