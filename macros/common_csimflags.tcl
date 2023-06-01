set common_csimflags {}

if {[info exists ::env(CSIMFLAGS)]} {
  lappend common_csimflags {*}"$::env(CSIMFLAGS)"
}
