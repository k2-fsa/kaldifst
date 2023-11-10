from typing import List, Union

from _kaldifst import (
    Lattice,
    StdVectorFst,
    convert_nbest_to_vector,
    lattice_scale,
    scale_lattice,
    shortest_path,
)


def lattice_to_nbest(
    lat: Union[Lattice, StdVectorFst],
    acoustic_scale: float = 1.0,
    lm_scale: float = 1.0,
    n: int = 1,
) -> List[Union[Lattice, StdVectorFst]]:
    """Work out N-best paths in lattices

    It implements
    https://github.com/kaldi-asr/kaldi/blob/master/src/latbin/lattice-to-nbest.cc

    Args:
      lat:
        The input lattice.
      acoustic_scale:
        Scaling factor for acoustic likelihoods.
      lm_scale:
        Scaling factor for language model scores.
      n:
        Number of distinct paths.
    Returns:
      Return a list of linear FSTs.

    **Example for a StdVectorFst**

    .. literalinclude:: ./code/lattice_to_nbest/ex1.py
       :language: python
       :linenos:
       :caption: lattice_to_nbest for a StdVectorFst

    .. figure:: ./code/lattice_to_nbest/vector-fst.svg
        :alt: vector-fst.svg
        :align: center
        :figwidth: 600px

        Visualization of vector-fst.svg

    .. figure:: ./code/lattice_to_nbest/vector-fst-3best-0.svg
        :alt: vector-fst-3best-0.svg
        :align: center
        :figwidth: 600px

        Visualization of vector-fst-3best-0.svg

    .. figure:: ./code/lattice_to_nbest/vector-fst-3best-1.svg
        :alt: vector-fst-3best-1.svg
        :align: center
        :figwidth: 600px

        Visualization of vector-fst-3best-1.svg

    .. figure:: ./code/lattice_to_nbest/vector-fst-3best-2.svg
        :alt: vector-fst-3best-2.svg
        :align: center
        :figwidth: 600px

        Visualization of vector-fst-3best-2.svg

    **Example for a Lattice**

    .. literalinclude:: ./code/lattice_to_nbest/ex2.py
       :language: python
       :linenos:
       :caption: lattice_to_nbest for a Lattice

    .. figure:: ./code/lattice_to_nbest/lattice.svg
        :alt: lattice.svg
        :align: center
        :figwidth: 600px

        Visualization of lattice.svg

    .. figure:: ./code/lattice_to_nbest/lattice-3best-0.svg
        :alt: lattice-3best-0.svg
        :align: center
        :figwidth: 600px

        Visualization of lattice-3best-0.svg

    .. figure:: ./code/lattice_to_nbest/lattice-3best-1.svg
        :alt: lattice-3best-1.svg
        :align: center
        :figwidth: 600px

        Visualization of lattice-3best-1.svg

    .. figure:: ./code/lattice_to_nbest/lattice-3best-2.svg
        :alt: lattice-3best-2.svg
        :align: center
        :figwidth: 600px

        Visualization of lattice-3best-2.svg
    """
    if lm_scale != 1 or acoustic_scale != 1:
        lat = type(lat)(lat)  # a deep copy
        lat_scale = lattice_scale(lmwt=lm_scale, acwt=acoustic_scale)
        scale_lattice(scale=lat_scale, in_out=lat)

    nbest = shortest_path(lat, n=n)
    return convert_nbest_to_vector(nbest)
