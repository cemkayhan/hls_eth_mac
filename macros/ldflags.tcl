set ldflags {}
lappend ldflags {-Wl,-z,stack-size=104857600}
#lappend ldflags {-L/usr/local/lib64}
#lappend ldflags {-lopencv_core}
#lappend ldflags {-lopencv_highgui}
#lappend ldflags {-lopencv_imgproc}
#lappend ldflags {-lopencv_imgcodecs}

if {[info exists ::env(LDFLAGS)]} {
  lappend ldflags {*}"$::env(LDFLAGS)"
}

