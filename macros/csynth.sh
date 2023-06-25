#!/bin/sh

source common_cflags.sh

CFLAGS=$CFLAGS vitis_hls -f csynth.tcl
