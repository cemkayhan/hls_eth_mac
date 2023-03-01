set target synth

source common_params.tcl
source open_project.tcl
source common_cflags.tcl
source add_files.tcl

csynth_design

puts {---------------------------------------------}
puts " PRJ_DIR: $prj_dir"
puts {---------------------------------------------}
exit
