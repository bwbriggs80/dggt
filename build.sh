#!/bin/sh

gcc -ggdb -std=c++17 ./src/dggt_math.cxx ./src/dggt_mem_utils.cxx ./src/dggt_allocator.cxx ./src/dggt.cxx -o dggt
