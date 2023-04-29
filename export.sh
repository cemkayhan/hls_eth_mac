#!/bin/sh

CFLAGS="-DD_ROW_=512 -DD_COL_=640 -DD_FIELD_BLANK_=200 -DD_LINE_BLANK_=100 -DD_PPC_=1 -DD_VID_DWIDTH_=16 -DD_AXIS_DWIDTH_=24" vitis_hls -f export_impl_ip.tcl
