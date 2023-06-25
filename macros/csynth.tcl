set target csynth

source open_project.tcl
source common_cflags.tcl
source add_files.tcl

source config_rtl.tcl
csynth_design

puts {---------------------------------------------}
puts " PRJ_DIR: $prj_dir"
puts {---------------------------------------------}
exit
