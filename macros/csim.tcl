set target csim

source open_project.tcl
source common_cflags.tcl
source common_csimflags.tcl
source ldflags.tcl
source add_files.tcl
source add_tb_files.tcl

csim_design -clean -O -ldflags $ldflags

puts {---------------------------------------------}
puts " PRJ_DIR: $prj_dir"
puts {---------------------------------------------}
exit
