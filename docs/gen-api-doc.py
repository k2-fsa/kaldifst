#!/usr/bin/env python3

"""
This file generates RST style documentation, automagically,
by reading the doc information from the module `kaldifst`.

Its usage in `Makefile` is:

    python3 ./gen-api-doc.py > source/python_api/api.rst

The generated `api.rst` is not checked into the git repository.
"""

import inspect
from typing import List

import kaldifst


def get_function_names(m_or_cls: object) -> List[str]:
    """Get function/method names within a module or class."""
    ans = []
    for m in inspect.getmembers(m_or_cls):
        if inspect.isroutine(m[1]):
            ans.append(m[0])
    ans.sort()
    return ans


def get_property_names(cls: object) -> List[str]:
    """Get property names of a class"""
    ans = []
    for m in inspect.getmembers(cls):
        if isinstance(m[1], property):
            ans.append(m[0])
    ans.sort()
    return ans


def get_class_names(module) -> List[str]:
    """Get class names of a module."""
    ans = []
    for m in inspect.getmembers(module):
        if inspect.isclass(m[1]):
            ans.append(m[0])
    ans.sort()
    return ans


def generate_doc_for_functions(names: List[str]) -> str:
    """Generate documentation for a list of functions."""
    ans = ""
    for n in names:
        ans += n
        ans += "\n"
        ans += "-" * len(n)
        ans += "\n\n"
        ans += f".. autofunction:: {n}\n\n"
    return ans


def generate_doc_for_classes(module, names: List[str]) -> str:
    """Generate documentation for a list of classes."""
    ans = ""
    for n in names:
        if n == "Lattice":
            ans += ".. _lattice_class:\n\n"

        if n == "LatticeArc":
            ans += ".. _lattice_arc_class:\n\n"

        if n == "LatticeWeight":
            ans += ".. _lattice_weight_class:\n\n"

        ans += n
        ans += "\n"
        ans += "-" * len(n)
        ans += "\n\n"
        if n == "Lattice":
            ans += (
                "Please refer to :ref:`introduction_to_lattice` for usage.\n\n"
            )

        cls = getattr(module, n)
        if hasattr(cls, "forward"):
            method_names = ["forward"]
        else:
            method_names = get_function_names(getattr(module, n))

        for m in method_names:
            m_obj = getattr(cls, m)
            if not hasattr(m_obj, "__doc__"):
                continue
            if m_obj.__doc__ is None:
                continue
            if m_obj.__doc__.count("\n") < 7 and m != "__str__":
                continue

            if m_obj.__doc__.count("\n") < 2:
                continue

            ans += m
            ans += "\n"
            ans += "^" * len(m)
            ans += "\n\n"

            ans += f".. automethod:: {module.__name__}.{cls.__name__}.{m}"
            ans += "\n\n"
        properties = get_property_names(cls)
        for p in properties:
            ans += p
            ans += "\n"
            ans += "^" * len(p)
            ans += "\n\n"
            if n == "LatticeWeight" and p == "value1":
                ans += "The graph cost.\n\n"

            if n == "LatticeWeight" and p == "value2":
                ans += "The acoustic cost.\n\n"

            ans += f".. autoattribute:: {module.__name__}.{cls.__name__}.{p}"
            ans += "\n\n"

    return ans


def main():
    doc = "kaldifst\n"
    doc += "=" * len("kaldifst")
    doc += "\n\n"

    doc += ".. currentmodule:: kaldifst"
    doc += "\n\n"

    function_names = get_function_names(kaldifst)
    doc += generate_doc_for_functions(function_names)

    class_names = get_class_names(kaldifst)
    doc += generate_doc_for_classes(kaldifst, class_names)

    print(doc)


if __name__ == "__main__":
    main()
