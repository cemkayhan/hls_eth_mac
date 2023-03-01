set target synth
set i 1
while {[file exists [set prj_dir ${target}_$i]]} {incr i}

open_project $prj_dir -reset
open_solution [set solution_dir solution1] -reset
set_part {xczu7ev-fbvb900-1-i}
set_top videoip_top
create_clock -period 200MHz

set common_cflags {}
#lappend common_cflags {-O3}
#lappend common_cflags {-std=c++14}
lappend common_cflags {-Wall}
lappend common_cflags {-Wextra}
#lappend common_cflags {-Wno-unused-parameter}
#lappend common_cflags {-Wno-unused-label}

set cflags $common_cflags
add_files videoip.cpp -cflags $cflags

csynth_design
file copy -force -- vivado_hls.log [file join $prj_dir $solution_dir]
puts {---------------------------------------------}
puts " PRJ_DIR: $prj_dir"
puts {---------------------------------------------}
exit
