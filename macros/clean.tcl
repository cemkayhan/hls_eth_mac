set targets {csim_* cosim_* csynth_* export_*}
file delete -force -- {*}[glob -type d -nocomplain {*}$targets]
