#!/bin/sh

source common_cflags.sh

CFLAGS=$CFLAGS vitis_hls -f export_impl_ip.tcl
