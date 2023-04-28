set cflags $common_cflags
#lappend cflags {-I/usr/local/include/opencv}
#lappend cflags {-I/usr/local/include/opencv -I/usr/local/include}
#lappend cflags {-I/usr/local/include/opencv4/opencv}
#lappend cflags {-I/usr/local/include/opencv4}
add_files -tb dahua_fifo_to_axis_tb.cpp -cflags $cflags
