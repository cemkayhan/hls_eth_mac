set cflags $common_cflags
if {[info exists common_csimflags]} {
  lappend cflags {*}"$common_csimflags"
}
if {[info exists common_cosimflags]} {
  lappend cflags {*}"$common_cosimflags"
}

set fid [open [file join .. FILES] r]
while {[gets $fid line]>=0} {
  add_files $line -cflags $cflags
}
