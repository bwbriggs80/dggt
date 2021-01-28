#!/bin/sh

buildname=$1
if [[ $buildname != "RELEASE" ]]; then
	buildname="DEBUG"
fi

g++ -ggdb -std=c++17 -D$buildname ./src/dggt_math.cxx ./src/dggt_mem_utils.cxx ./src/dggt_allocator.cxx ./src/dggt_test.cxx -o dggt
