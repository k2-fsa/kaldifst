## Introduction

Python wrapper for [OpenFst][openfst] and its extensions from  [Kaldi][kaldi], e.g.,
`fstdeterminizestar`.

**Note**: It does not depend on Kaldi. You don't need to install
Kaldi to use this repo.

## Installation

You can install it via `pip`:

```
pip install --verbose kaldifst
```

To check that `kaldifst` was installed successfully, you can use

```
python3 -c "import kaldifst; print(kaldifst.__version__)"
```
which should print the version of installed `kaldifst`.

## Usage

This is still an on-going project. Please refer to

<https://github.com/csukuangfj/kaldifst/tree/master/kaldifst/python/tests>

for usages.

We will add usage documentation later.

[openfst]: https://www.openfst.org/
[kaldi]: https://github.com/kaldi-asr/kaldi
