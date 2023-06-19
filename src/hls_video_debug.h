#if !defined(HLS_VIDEO_DEBUG_H_INCLUDE_GUARD_)
#define HLS_VIDEO_DEBUG_H_INCLUDE_GUARD_

#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include "bit_width.h"
#include "bit_power.h"

void Hls_Video_Debug(
  hls::stream<ap_axiu<Bit_Power_Num<2*D_PIX_DEPTH_*D_PPC_>::Value,1,1,1> >& Vid_In,
  hls::stream<ap_axiu<Bit_Power_Num<2*D_PIX_DEPTH_*D_PPC_>::Value,1,1,1> >& Vid_Out,
  ap_uint<16>& Pixel_Cntr_Axi,
  ap_uint<16>& Line_Cntr_Axi,
  ap_uint<16>& Frame_Cntr_Axi,
  volatile ap_uint<16>* Pixel_Cntr,
  volatile ap_uint<16>* Line_Cntr,
  volatile ap_uint<16>* Frame_Cntr);

#endif
