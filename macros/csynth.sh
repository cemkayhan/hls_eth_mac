#!/bin/sh

CFLAGS="-DD_MAX_MTU_=1500 -DD_MIN_MTU_=46" vitis_hls -f csynth.tcl
