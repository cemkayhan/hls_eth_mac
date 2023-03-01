set i 1
while {[file exists [set prj_dir ${target}_$i]]} {incr i}

open_project $prj_dir -reset
open_solution solution1 -reset
set_part $part
set_top $top
create_clock -period $period
