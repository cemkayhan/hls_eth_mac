set target cosim
set i 1
while {[file exists [set prj_dir ${target}_$i]]} {incr i}

####################################################
set WIDTH 1920
####################################################

open_project $prj_dir -reset
open_solution [set solution_dir solution1] -reset
set_part {xczu7ev-fbvb900-1-i}
set_top video_offset
create_clock -period 200MHz

set common_cflags {}
lappend common_cflags {-O3}
lappend common_cflags {-std=c++0x}
lappend common_cflags {-Wall}
lappend common_cflags {-Wextra}
#lappend common_cflags {-Wno-unused-parameter}
#lappend common_cflags {-Wno-unused-label}

set cflags $common_cflags
lappend cflags "-DWIDTH_=${WIDTH}"
add_files video_offset.cpp -cflags $cflags

set cflags $common_cflags
#lappend cflags {-I/usr/local/include/opencv}
#lappend cflags {-I/usr/local/include/opencv -I/usr/local/include}
lappend cflags {-I/usr/local/include/opencv4/opencv}
lappend cflags {-I/usr/local/include/opencv4}
lappend cflags "-DWIDTH_=${WIDTH}"
add_files -tb video_offset_tb.cpp -cflags $cflags

csynth_design
#file copy -force -- vivado_hls.log [file join $prj_dir $solution_dir]

set ldflags {}
lappend ldflags {-L/usr/local/lib64}
lappend ldflags {-lopencv_core}
lappend ldflags {-lopencv_highgui}
lappend ldflags {-lopencv_imgproc}
lappend ldflags {-lopencv_imgcodecs}
cosim_design -O -setup -ldflags $ldflags

puts {---------------------------------------------}
puts " PRJ_DIR: $prj_dir"
puts {---------------------------------------------}
exit
