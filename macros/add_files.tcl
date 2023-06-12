set cflags $common_cflags
if {[info exists common_csimflags]} {
  lappend cflags {*}"$common_csimflags"
}
if {[info exists common_cosimflags]} {
  lappend cflags {*}"$common_cosimflags"
}
add_files [file join .. src hls_video_debug.cpp] -cflags $cflags
