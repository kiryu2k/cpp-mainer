#!/bin/bash

readonly scriptdir=$(realpath $(dirname $0))

LD_LIBRARY_PATH=${scriptdir}/../build/debug/src/libgeometry-c/ \
    python3 ${scriptdir}/main.py
