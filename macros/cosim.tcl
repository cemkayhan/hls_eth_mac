set target cosim

source open_project.tcl
source common_cflags.tcl
source common_cosimflags.tcl
source ldflags.tcl
source add_files.tcl
source add_tb_files.tcl

source config_rtl.tcl
source config_dataflow.tcl
csynth_design
cosim_design -O -ldflags $ldflags -trace_level [gets [open [file join .. TRACE_LEVEL] r]]

puts {---------------------------------------------}
puts " PRJ_DIR: $prj_dir"
puts {---------------------------------------------}
exit
