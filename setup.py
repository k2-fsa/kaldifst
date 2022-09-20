#!/usr/bin/env python3
#
# Copyright (c)  2020  Xiaomi Corporation (author: Fangjun Kuang)

import glob
import os
import platform
import re
import shutil
import sys

import setuptools
from setuptools.command.build_ext import build_ext

cur_dir = os.path.dirname(os.path.abspath(__file__))


def is_windows():
    return platform.system() == "Windows"


def cmake_extension(name, *args, **kwargs) -> setuptools.Extension:
    kwargs["language"] = "c++"
    sources = []
    return setuptools.Extension(name, sources, *args, **kwargs)


class BuildExtension(build_ext):
    def build_extension(self, ext: setuptools.extension.Extension):
        build_dir = self.build_temp
        os.makedirs(build_dir, exist_ok=True)

        # build/lib.linux-x86_64-3.8
        os.makedirs(self.build_lib, exist_ok=True)

        kaldifst_dir = os.path.dirname(os.path.abspath(__file__))

        cmake_args = os.environ.get("KALDIFST_CMAKE_ARGS", "")
        make_args = os.environ.get("KALDIFST_MAKE_ARGS", "")
        system_make_args = os.environ.get("MAKEFLAGS", "")

        if cmake_args == "":
            cmake_args = "-DCMAKE_BUILD_TYPE=Release"

        if make_args == "" and system_make_args == "":
            print("For fast compilation, run:")
            print('export KALDIFST_MAKE_ARGS="-j"; python setup.py install')

        if "PYTHON_EXECUTABLE" not in cmake_args:
            print(f"Setting PYTHON_EXECUTABLE to {sys.executable}")
            cmake_args += f" -DPYTHON_EXECUTABLE={sys.executable}"

        if not is_windows():
            build_cmd = f"""
                cd {self.build_temp}

                cmake {cmake_args} {kaldifst_dir}

                make {make_args} _kaldifst
            """
            print(f"build command is:\n{build_cmd}")

            ret = os.system(build_cmd)
            if ret != 0:
                raise Exception(
                    "\nBuild kaldifst failed. Please check the error message.\n"
                    "You can ask for help by creating an issue on GitHub.\n"
                    "\nClick:\n"
                    "    https://github.com/csukuangfj/kaldifst/issues/new\n"
                )
            lib_so = glob.glob(f"{build_dir}/lib/*.so*")

            # macos
            # also need to copy *fst*.dylib
            lib_so += glob.glob(f"{build_dir}/lib/*.dylib*")

            for so in lib_so:
                print(f"Copying {so} to {self.build_lib}/")
                shutil.copy(f"{so}", f"{self.build_lib}/")
            return

        # for windows
        build_cmd = f"""
            cmake {cmake_args} -B {self.build_temp} -S {cur_dir}
            cmake --build {self.build_temp} --target _kaldifst --config Release -- -m
        """
        print(f"build command is:\n{build_cmd}")

        ret = os.system(f"cmake {cmake_args} -B {self.build_temp} -S {cur_dir}")
        if ret != 0:
            raise Exception("Failed to build kaldifst")

        ret = os.system(
            f"cmake --build {self.build_temp} --target _kaldifst --config Release -- -m"
        )
        if ret != 0:
            raise Exception("Failed to build kaldifst")

        # bin/Release/_kaldifst.cp38-win_amd64.pyd
        lib_so = glob.glob(f"{self.build_temp}/**/*.pyd", recursive=True)

        # lib/Release/{_kaldifst, openfst}.lib
        lib_so += glob.glob(f"{self.build_temp}/**/*.lib", recursive=True)

        for so in lib_so:
            print(f"Copying {so} to {self.build_lib}/")
            shutil.copy(f"{so}", f"{self.build_lib}/")


def read_long_description():
    with open("README.md", encoding="utf8") as f:
        readme = f.read()
    return readme


def get_package_version():
    with open("CMakeLists.txt") as f:
        content = f.read()

    latest_version = re.search(r"set\(KALDIFST_VERSION (.*)\)", content).group(
        1
    )
    latest_version = latest_version.strip('"')
    return latest_version


with open("kaldifst/python/kaldifst/__init__.py", "a") as f:
    f.write(f"__version__ = '{get_package_version()}'\n")


package_name = "kaldifst"

setuptools.setup(
    name=package_name,
    version=get_package_version(),
    author="Fangjun Kuang",
    author_email="csukuangfj@gmail.com",
    package_dir={
        package_name: "kaldifst/python/kaldifst",
        "kaldifst.utils": "kaldifst/python/kaldifst/utils",
    },
    packages=[package_name, "kaldifst.utils"],
    url="https://github.com/csukuangfj/kaldifst",
    long_description=read_long_description(),
    long_description_content_type="text/markdown",
    ext_modules=[cmake_extension("_kaldifst")],
    cmdclass={"build_ext": BuildExtension},
    zip_safe=False,
    classifiers=[
        "Programming Language :: C++",
        "Programming Language :: Python",
        "Topic :: Scientific/Engineering :: Artificial Intelligence",
    ],
    license="Apache licensed, as found in the LICENSE file",
)

# remove the line __dev_version__ from k2/python/k2/__init__.py
with open("kaldifst/python/kaldifst/__init__.py", "r") as f:
    lines = f.readlines()

with open("kaldifst/python/kaldifst/__init__.py", "w") as f:
    for line in lines:
        if "__version__" not in line:
            f.write(line)
