set targets {sim_* cosim_* synth_* export_*}
file delete -force -- {*}[glob -type d -nocomplain {*}$targets]
