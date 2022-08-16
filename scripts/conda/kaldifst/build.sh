#!/usr/bin/env bash
#
# Copyright      2021  Xiaomi Corp.       (author: Fangjun Kuang)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -ex

CONDA_ENV_DIR=$CONDA_PREFIX

python3 --version

echo "CC is: $CC"
echo "GCC is: $GCC"
echo "gcc version: $($CC --version)"

export KALDI_NATIVE_IO_CMAKE_ARGS="-DCMAKE_BUILD_TYPE=Release"
export KALDI_NATIVE_IO_MAKE_ARGS="-j"

python3 setup.py install --single-version-externally-managed --record=record.txt
