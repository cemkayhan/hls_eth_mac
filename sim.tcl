set target sim
set i 1
while {[file exists [set prj_dir ${target}_$i]]} {incr i}

open_project $prj_dir -reset
open_solution [set solution_dir solution1] -reset
set_part {xczu7ev-fbvb900-1-i}
set_top video_offset
create_clock -period 300MHz

set common_cflags {}
#lappend common_cflags {-O3}
#lappend common_cflags {-std=c++14}
lappend common_cflags {-Wall}
lappend common_cflags {-Wextra}
#lappend common_cflags {-Wno-unused-parameter}
#lappend common_cflags {-Wno-unused-label}

set cflags $common_cflags
add_files video_offset.cpp -cflags $cflags

set cflags $common_cflags
lappend cflags {-I/user/include/}
add_files -tb video_offset_tb.cpp -cflags $cflags

csim_design -O
puts {---------------------------------------------}
puts " PRJ_DIR: $prj_dir"
puts {---------------------------------------------}
exit
