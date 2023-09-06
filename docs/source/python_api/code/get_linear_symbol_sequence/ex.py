import graphviz

import kaldifst

s = """
0 1 0 2 0.5
1 2 1 3 0.8
2 3 3 5 0.7
3 4 9 0 0.1
4 0.2
"""


fst = kaldifst.compile(s=s, acceptor=False)

(
    succeeded,
    isymbols_out,
    osymbols_out,
    total_weight,
) = kaldifst.get_linear_symbol_sequence(fst)
assert succeeded is True
assert isymbols_out == [1, 3, 9]
assert osymbols_out == [2, 3, 5]

print(dir(total_weight))
assert (
    abs(total_weight.value - (0.5 + 0.8 + 0.7 + 0.1 + 0.2)) < 1e-3
), total_weight.value
