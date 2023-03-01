set cflags $common_cflags
#lappend cflags {-I/usr/local/include/opencv}
#lappend cflags {-I/usr/local/include/opencv -I/usr/local/include}
lappend cflags {-I/usr/local/include/opencv4/opencv}
lappend cflags {-I/usr/local/include/opencv4}
lappend cflags "-DWIDTH_=${width}"
add_files -tb video_offset_tb.cpp -cflags $cflags
