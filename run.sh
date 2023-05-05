#!/bin/bash

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j5
make coverage_report -j5
