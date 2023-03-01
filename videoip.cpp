#include "videoip.h"

void videoip_top(hls::stream<ap_axis<96,1,1,1> >& insrc1, 
                 hls::stream<ap_axis<96,1,1,1> >& outsrc1)
{
//#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=insrc1
#pragma HLS INTERFACE axis port=outsrc1
#pragma HLS PIPELINE

  videoip<96>(insrc1,outsrc1);
}
