#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"

constexpr auto WIDTH = 1920;

template<int WIDTH>
void fillLineBuffers(ap_uint<24> lineBuffer1[WIDTH],
		ap_uint<24> lineBuffer2[WIDTH],
		hls::stream<ap_axiu<24,1,0,0>>& vidIn,
		hls::stream<ap_uint<1>>& vidUser)
{
	ap_axiu<24,1,0,0> tmp = vidIn.read();

	// Save TUSER
	vidUser.write(tmp.user);
	lineBuffer1[0] = tmp.data;

	for (auto i=1; i<WIDTH; ++i) {
		lineBuffer1[i] = vidIn.read().data;
	}
	for (auto i=0; i<WIDTH; ++i) {
		lineBuffer2[i] = vidIn.read().data;
	}
}

template<int WIDTH>
void readLineWithOffset(ap_uint<24> line[WIDTH],
		hls::stream<ap_uint<24>>& vidOut,
		int offset)
{
	for (auto i=offset; i<WIDTH; ++i) {
		vidOut.write(line[i]);
	}
	for (auto i=0; i<offset; ++i) {
		vidOut.write(line[i]);
	}
}

template<int WIDTH>
void drainLineBuffers(hls::stream<ap_uint<24>>& vidOut1,
		hls::stream<ap_uint<24>>& vidOut2,
		hls::stream<ap_axiu<24,1,0,0>>& vidOut,
		hls::stream<ap_uint<1>>& vidUser)
{
	ap_axiu<24,1,0,0> pix;

	// Add TUSER
	pix.user = vidUser.read();
	pix.data = vidOut1.read();
	pix.last = 0;
	vidOut.write(pix);

	loppVidOut1: for (auto i=0; i<WIDTH-2; ++i) {
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

	loopVidOut2: for (auto i=0; i<WIDTH-1; ++i) {
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

void video_offset(hls::stream<ap_axiu<24,1,0,0>>& vidIn,
		hls::stream<ap_axiu<24,1,0,0>>& vidOut,
		int offset)
{
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE axis port=vidIn
#pragma HLS INTERFACE axis port=vidOut

#pragma HLS STABLE variable=offset

#pragma HLS DATAFLOW

	hls::stream<ap_uint<24>> vidOut1;
#pragma HLS STREAM variable=vidOut1 depth=2048

	hls::stream<ap_uint<24>> vidOut2;
#pragma HLS STREAM variable=vidOut2 depth=2048

	hls::stream<ap_uint<1>> vidUser;
#pragma HLS STREAM variable=vidUser depth=2048

	ap_uint<24> lineBuffer1[WIDTH];
	ap_uint<24> lineBuffer2[WIDTH];

	fillLineBuffers<WIDTH>(lineBuffer1,lineBuffer2,vidIn,vidUser);
	readLineWithOffset<WIDTH>(lineBuffer1,vidOut1,offset);
	readLineWithOffset<WIDTH>(lineBuffer2,vidOut2,offset);
	drainLineBuffers<WIDTH>(vidOut1,vidOut2,vidOut,vidUser);
}
