set common_cosimflags {}

if {[info exists ::env(COSIMFLAGS)]} {
  lappend common_cosimflags {*}"$::env(COSIMFLAGS)"
}
