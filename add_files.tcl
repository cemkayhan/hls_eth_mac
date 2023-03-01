set cflags $common_cflags
lappend cflags "-DWIDTH_=${width}"
add_files video_offset.cpp -cflags $cflags
