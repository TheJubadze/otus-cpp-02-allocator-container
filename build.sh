#!/bin/sh

git clean -xdf

cmake -S ./src -B build -DPATCH_VERSION=1
cmake --build build

cmake --build ./build --target package