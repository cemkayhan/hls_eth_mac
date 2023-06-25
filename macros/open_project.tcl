set i 1
while {[file exists [set prj_dir ${target}_$i]]} {incr i}

open_project $prj_dir -reset
open_solution solution1 -reset -flow_target vivado
set_part [gets [open [file join .. PART] r]]
set_top [lindex [split [lsearch -regexp -inline $::env(CFLAGS) -DD_TOP_=] =] 1]
create_clock -period [gets [open [file join .. CLKPERIOD] r]]
