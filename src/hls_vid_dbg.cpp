#include "hls_vid_dbg.h"

void D_TOP_(
  hls::stream<ap_axiu<Axis_Vid_Busw<D_PIX_DEPTH_,D_PPC_>::Value,1,1,1> >& Vid_In,
  hls::stream<ap_axiu<Axis_Vid_Busw<D_PIX_DEPTH_,D_PPC_>::Value,1,1,1> >& Vid_Out,
  ap_uint<16>& Pixel_Cntr_Axi,
  ap_uint<16>& Line_Cntr_Axi,
  ap_uint<16>& Frame_Cntr_Axi,
  ap_uint<16>& Pixel_Cntr,
  ap_uint<16>& Line_Cntr,
  ap_uint<16>& Frame_Cntr
){
#pragma HLS INTERFACE s_axilite bundle=Ctrl port=return
#pragma HLS INTERFACE s_axilite bundle=Ctrl port=Pixel_Cntr_Axi offset=0x10
#pragma HLS INTERFACE s_axilite bundle=Ctrl port=Line_Cntr_Axi offset=0x18
#pragma HLS INTERFACE s_axilite bundle=Ctrl port=Frame_Cntr_Axi offset=0x20

#pragma HLS INTERFACE ap_none port=Pixel_Cntr
#pragma HLS INTERFACE ap_none port=Line_Cntr
#pragma HLS INTERFACE ap_none port=Frame_Cntr

#pragma HLS INTERFACE axis port=Vid_In
#pragma HLS INTERFACE axis port=Vid_Out

  static ap_uint<16> Pix_Cntr_=0;
#pragma HLS RESET variable=Pix_Cntr_

  static ap_uint<16> Line_Cntr_=0;
#pragma HLS RESET variable=Line_Cntr_

  static ap_uint<16> Frame_Cntr_=0;
#pragma HLS RESET variable=Frame_Cntr_

#pragma HLS PIPELINE

  ap_axiu<Axis_Vid_Busw<D_PIX_DEPTH_,D_PPC_>::Value,1,1,1> Pix;
  Vid_In >> Pix;
  Vid_Out << Pix;

  if(Pix.last){
    Pix_Cntr_=0;
    ++Line_Cntr_;
  }else if(Pix.user){
    ++Pix_Cntr_;
    Line_Cntr_=0;
    ++Frame_Cntr_;
  }else{
    ++Pix_Cntr_;
  }

  Pixel_Cntr=Pix_Cntr_;
  Pixel_Cntr_Axi=Pix_Cntr_;

  Line_Cntr=Line_Cntr_;
  Line_Cntr_Axi=Line_Cntr_;

  Frame_Cntr=Frame_Cntr_;
  Frame_Cntr_Axi=Frame_Cntr_;
}
