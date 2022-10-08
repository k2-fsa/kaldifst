# Copyright      2022  Xiaomi Corp.        (authors: Fangjun Kuang)
# See ../../../LICENSE for clarification regarding multiple authors


from typing import Any, Tuple

from _kaldifst import (
    _RandomAccessVectorFstReader,
    _SequentialVectorFstReader,
    _VectorFstWriter,
)


class _TableWriter(object):
    def __init__(self, wspecifier: str) -> None:
        """
        Args:
          wspecifier:
            Kaldi table wspecifier.
        """
        # The subclass should instantiate `_impl`
        self._impl = None
        self.open(wspecifier)

    def open(self, wspecifier: str) -> None:
        """The subclass should implement this function
        and instantiate `self._impl`

        Args:
          wspecifier:
            Kaldi table wspecifier.
        """
        raise NotImplementedError

    @property
    def is_open(self) -> bool:
        """Return ``True`` if it is opened; return ``False`` otherwise."""
        return self._impl.is_open

    def write(self, key: str, value: Any) -> None:
        """Write an item.

        Args:
          key:
            The key for the item.
          value:
            The value for the item. Its type depends on the actual writer class.
        """
        self._impl.write(key, value)

    def flush(self) -> None:
        self._impl.flush()

    def close(self) -> None:
        if self.is_open:
            self._impl.close()

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback) -> None:
        self.close()

    def __setitem__(self, key: str, value: Any) -> None:
        self.write(key, value)

    def __del__(self) -> None:
        self.close()


class _SequentialTableReader(object):
    def __init__(self, rspecifier: str) -> None:
        """
        Args:
          rspecifier:
            Kaldi table rspecifier.
        """
        # The subclass should instantiate `_impl`
        self._impl = None
        self.open(rspecifier)

    def open(self, rspecifier: str) -> None:
        """The subclass should implement this function
        and instantiate `self._impl`

        Args:
          rspecifier:
            Kaldi table rspecifier.
        """
        raise NotImplementedError

    @property
    def is_open(self) -> bool:
        """Return ``True`` if it is opened; return ``False`` otherwise."""
        return self._impl.is_open

    @property
    def done(self) -> bool:
        return self._impl.done

    @property
    def key(self) -> str:
        return self._impl.key

    @property
    def value(self) -> Any:
        return self._impl.value

    def free_current(self) -> None:
        self._impl.free_current()

    def next(self) -> None:
        self._impl.next()

    def close(self) -> None:
        if self.is_open:
            self._impl.close()

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback) -> None:
        if self.is_open:
            self.close()

    def __iter__(self) -> Tuple[str, Any]:
        while not self.done:
            key = self.key
            value = self.value
            yield key, value
            self.next()

    def __del__(self) -> None:
        self.close()


class _RandomAccessTableReader(object):
    def __init__(self, rspecifier: str) -> None:
        """
        Args:
          rspecifier:
            Kaldi table rspecifier.
        """
        # The subclass should instantiate `_impl`
        self._impl = None
        self.open(rspecifier)

    def open(self, rspecifier: str) -> None:
        """The subclass should implement this function
        and instantiate `self._impl`

        Args:
          rspecifier:
            Kaldi table rspecifier.
        """
        raise NotImplementedError

    @property
    def is_open(self) -> bool:
        """Return ``True`` if it is opened; return ``False`` otherwise."""
        return self._impl.is_open

    def close(self) -> None:
        if self.is_open:
            self._impl.close()

    def __contains__(self, key: str) -> bool:
        return key in self._impl

    def __getitem__(self, key) -> Any:
        """The actual return type depends on the type of `self._impl`."""
        return self._impl[key]

    def __enter__(self):
        return self

    def __exit__(self, type, value, traceback) -> None:
        if self.is_open:
            self.close()

    def __del__(self) -> None:
        self.close()


class VectorFstWriter(_TableWriter):
    def open(self, wspecifier: str) -> None:
        self._impl = _VectorFstWriter(wspecifier)


class SequentialVectorFstReader(_SequentialTableReader):
    def open(self, rspecifier: str) -> None:
        self._impl = _SequentialVectorFstReader(rspecifier)


class RandomAccessVectorFstReader(_RandomAccessTableReader):
    def open(self, rspecifier: str) -> None:
        self._impl = _RandomAccessVectorFstReader(rspecifier)
