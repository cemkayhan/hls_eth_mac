set ldflags {}
lappend ldflags {-L/usr/local/lib64}
lappend ldflags {-lopencv_core}
lappend ldflags {-lopencv_highgui}
lappend ldflags {-lopencv_imgproc}
lappend ldflags {-lopencv_imgcodecs}
