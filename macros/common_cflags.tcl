set common_cflags {}
lappend common_cflags {-O3}
#lappend common_cflags {-std=c++14}
lappend common_cflags {-Wall}
lappend common_cflags {-Wextra}
#lappend common_cflags {-Wno-unused-parameter}
lappend common_cflags {-Wno-unused-label}

if {[info exists ::env(CFLAGS)]} {
  lappend common_cflags {*}"$::env(CFLAGS)"
}
