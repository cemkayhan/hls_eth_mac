#include "video_offset.h"

void video_offset(hls::stream<ap_axiu<24,1,1,1> >& vidIn,
		hls::stream<ap_axiu<24,1,1,1> >& vidOut,
		int offset)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=vidIn
#pragma HLS INTERFACE axis port=vidOut
#pragma HLS INTERFACE ap_stable port=offset
#pragma HLS DATAFLOW

	hls::stream<ap_uint<24> > vidOut1;
#pragma HLS STREAM variable=vidOut1 depth=2048

	hls::stream<ap_uint<24> > vidOut2;
#pragma HLS STREAM variable=vidOut2 depth=2048

	hls::stream<ap_uint<1> > vidUser;
#pragma HLS STREAM variable=vidUser depth=2048

	ap_uint<24> lineBuffer1[WIDTH_];
	ap_uint<24> lineBuffer2[WIDTH_];

	fillLineBuffers<WIDTH_>(lineBuffer1,lineBuffer2,vidIn,vidUser);
	readLineWithOffset<0,WIDTH_>(lineBuffer1,vidOut1,offset);
	readLineWithOffset<1,WIDTH_>(lineBuffer2,vidOut2,offset);
	drainLineBuffers<WIDTH_>(vidOut1,vidOut2,vidOut,vidUser);
}
