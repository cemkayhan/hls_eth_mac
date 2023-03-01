#ifndef VIDEO_OFFSET_INCLUDE_GUARD_
#define VIDEO_OFFSET_INCLUDE_GUARD_

#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"

void video_offset(hls::stream<ap_axiu<24,1,1,1> >& vidIn,
		hls::stream<ap_axiu<24,1,1,1> >& vidOut,
		int offset);

template<int WIDTH>
void fillLineBuffers(ap_uint<24> lineBuffer1[WIDTH],
		ap_uint<24> lineBuffer2[WIDTH],
		hls::stream<ap_axiu<24,1,1,1> >& vidIn,
		hls::stream<ap_uint<1> >& vidUser)
{
	ap_axiu<24,1,1,1> tmp = vidIn.read();

	// Save TUSER
	vidUser.write(tmp.user);
	lineBuffer1[0] = tmp.data;

  loopfillBuffer1: for (int i=1; i<WIDTH; ++i) {
#pragma HLS PIPELINE
		lineBuffer1[i] = vidIn.read().data;
	}
	loopfillBuffer2: for (int i=0; i<WIDTH; ++i) {
#pragma HLS PIPELINE
		lineBuffer2[i] = vidIn.read().data;
	}
}

template<int ID,int WIDTH>
void readLineWithOffset(ap_uint<24> line[WIDTH],
		hls::stream<ap_uint<24> >& vidOut,
		int offset)
{
	loopReadBuffer1: for (int i=offset; i<WIDTH; ++i) {
#pragma HLS PIPELINE
#pragma HLS LOOP_TRIPCOUNT min=WIDTH/2 max=WIDTH/2
		vidOut.write(line[i]);
	}
	loopReadBuffer2: for (int i=0; i<offset; ++i) {
#pragma HLS PIPELINE
#pragma HLS LOOP_TRIPCOUNT min=WIDTH/2 max=WIDTH/2
		vidOut.write(line[i]);
	}
}

template<int WIDTH>
void drainLineBuffers(hls::stream<ap_uint<24> >& vidOut1,
		hls::stream<ap_uint<24> >& vidOut2,
		hls::stream<ap_axiu<24,1,1,1> >& vidOut,
		hls::stream<ap_uint<1> >& vidUser)
{
	ap_axiu<24,1,1,1> pix;

	// Add TUSER
	pix.user = vidUser.read();
	pix.data = vidOut1.read();
	pix.last = 0;
	vidOut.write(pix);

	loppVidOut1: for (int i=0; i<WIDTH-2; ++i) {
#pragma HLS PIPELINE
		pix.user = 0;
		pix.data = vidOut1.read();
		pix.last = 0;
		vidOut.write(pix);
	}

	// Add TLAST
	pix.user = 0;
	pix.data = vidOut1.read();
	pix.last = 1;
	vidOut.write(pix);

	loopVidOut2: for (int i=0; i<WIDTH-1; ++i) {
#pragma HLS PIPELINE
		pix.user = 0;
		pix.data = vidOut2.read();
		pix.last = 0;
		vidOut.write(pix);
	}

	// Add TLAST
	pix.user = 0;
	pix.data = vidOut2.read();
	pix.last = 1;
	vidOut.write(pix);
}

#endif
