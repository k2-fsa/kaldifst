# Copyright      2021  Xiaomi Corp.        (authors: Fangjun Kuang)
# See ../../../LICENSE for clarification regarding multiple authors

from _kaldifst import (
    StdFst,
    StdVectorFst,
    _ArcIteratorStdVectorFst,
    _StateIteratorStdVectorFst,
)


class StateIterator(object):
    def __init__(self, fst: StdFst):
        if isinstance(fst, StdVectorFst):
            self._impl = _StateIteratorStdVectorFst(fst)
        else:
            raise f"Unsupported type: {type(fst)}"

    def __iter__(self):
        while not self._impl.done:
            yield self._impl.value
            self._impl.next()

    @property
    def done(self):
        return self._impl.done

    @property
    def value(self):
        return self._impl.value

    def next(self):
        self._impl.next()

    def reset(self):
        self._impl.reset()


class ArcIterator(object):
    def __init__(self, fst: StdFst, state: int):
        if isinstance(fst, StdVectorFst):
            self._impl = _ArcIteratorStdVectorFst(fst, state)
        else:
            raise ValueError(f"Unsupported type: {type(fst)}")

    def __iter__(self):
        while not self._impl.done:
            yield self._impl.value
            self._impl.next()

    @property
    def done(self):
        return self._impl.done

    @property
    def value(self):
        return self._impl.value

    @property
    def position(self):
        return self._impl.position

    @property
    def flags(self):
        return self._impl.flags

    def next(self):
        return self._impl.next()

    def reset(self):
        return self._impl.reset()

    def seek(self, a: int):
        return self._impl.seek(a)
