#!/bin/sh

conan remote add conan-center "https://conan.bintray.com"

git clone https://github.com/darcamo/conan-cxxopts || exit 1
cd conan-cxxopts && conan create . local/stable || exit 1
cd .. && rm -rf conan-cxxopts

git clone https://github.com/msaf1980/conan-plog || exit 1
cd conan-plog && conan create . local/stable || exit 1
cd .. && rm -rf conan-plog

git submodule init
git submodule update
