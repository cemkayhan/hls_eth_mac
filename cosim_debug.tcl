set target cosim

source common_params.tcl
source open_project.tcl
source common_cflags.tcl
source ldflags.tcl
source add_files.tcl
source add_tb_files.tcl

source config_rtl.tcl
csynth_design
cosim_design -ldflags $ldflags -trace_level $trace_level

puts {---------------------------------------------}
puts " PRJ_DIR: $prj_dir"
puts {---------------------------------------------}
exit
