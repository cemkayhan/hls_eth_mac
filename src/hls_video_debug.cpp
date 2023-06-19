#include "hls_video_debug.h"

template<int PIX_DEPTH_, int PPC_>
static void Main_Func(hls::stream<ap_axiu<Bit_Power_Num<2*D_PIX_DEPTH_*D_PPC_>::Value,1,1,1> >& Vid_In,
                      hls::stream<ap_axiu<Bit_Power_Num<2*D_PIX_DEPTH_*D_PPC_>::Value,1,1,1> >& Vid_Out,
                      ap_uint<16>& Pixel_Cntr_Axi,
                      ap_uint<16>& Line_Cntr_Axi,
                      ap_uint<16>& Frame_Cntr_Axi,
                      volatile ap_uint<16>* Pixel_Cntr,
                      volatile ap_uint<16>* Line_Cntr,
                      volatile ap_uint<16>* Frame_Cntr
) {
#pragma HLS INLINE

  ap_uint<16> Pixel_Cntr_=0;
  ap_uint<16> Line_Cntr_=0;
  ap_uint<16> Frame_Cntr_=-1;

  loopMainLoop: do{
#pragma HLS PIPELINE
    ap_axiu<Bit_Power_Num<2*D_PIX_DEPTH_*D_PPC_>::Value,1,1,1> Pix;
    Vid_In >> Pix;
    if(Pix.last){
#if defined(D_ENABLE_LINE_CNTR_)
      ++Line_Cntr_;
#endif
#if defined(D_ENABLE_PIXEL_CNTR_)
      Pixel_Cntr_=0;
#endif
    } else if(Pix.user){  
#if defined(D_ENABLE_FRAME_CNTR_)
      ++Frame_Cntr_;
#endif
#if defined(D_ENABLE_LINE_CNTR_)
      Line_Cntr_=0;
#endif
#if defined(D_ENABLE_PIXEL_CNTR_)
      ++Pixel_Cntr_;
#endif
    } else {
#if defined(D_ENABLE_PIXEL_CNTR_)
      ++Pixel_Cntr_;
#endif
    }

#if defined(D_ENABLE_PIXEL_CNTR_)
    Pixel_Cntr_Axi=Pixel_Cntr_;
    *Pixel_Cntr=Pixel_Cntr_;
#endif
#if defined(D_ENABLE_LINE_CNTR_)
    Line_Cntr_Axi=Line_Cntr_;
    *Line_Cntr=Line_Cntr_;
#endif
#if defined(D_ENABLE_FRAME_CNTR_)
    Frame_Cntr_Axi=Frame_Cntr_;
    *Frame_Cntr=Frame_Cntr_;
#endif
    Vid_Out << Pix;
  }while(1);
}

void Hls_Video_Debug(hls::stream<ap_axiu<Bit_Power_Num<2*D_PIX_DEPTH_*D_PPC_>::Value,1,1,1> >& Vid_In,
                     hls::stream<ap_axiu<Bit_Power_Num<2*D_PIX_DEPTH_*D_PPC_>::Value,1,1,1> >& Vid_Out,
                     ap_uint<16>& Pixel_Cntr_Axi,
                     ap_uint<16>& Line_Cntr_Axi,
                     ap_uint<16>& Frame_Cntr_Axi,
                     volatile ap_uint<16>* Pixel_Cntr,
                     volatile ap_uint<16>* Line_Cntr,
                     volatile ap_uint<16>* Frame_Cntr
) {
#pragma HLS INTERFACE s_axilite bundle=Ctrl port=return
#pragma HLS INTERFACE s_axilite bundle=Ctrl port=Pixel_Cntr_Axi offset=0x10
#pragma HLS INTERFACE s_axilite bundle=Ctrl port=Line_Cntr_Axi offset=0x18
#pragma HLS INTERFACE s_axilite bundle=Ctrl port=Frame_Cntr_Axi offset=0x20

#pragma HLS INTERFACE ap_none port=Pixel_Cntr
#pragma HLS INTERFACE ap_none port=Line_Cntr
#pragma HLS INTERFACE ap_none port=Frame_Cntr

#pragma HLS INTERFACE axis port=Vid_In
#pragma HLS INTERFACE axis port=Vid_Out

  Main_Func<D_PIX_DEPTH_,D_PPC_>
    (Vid_In,Vid_Out,
     Pixel_Cntr_Axi,Line_Cntr_Axi,Frame_Cntr_Axi,
     Pixel_Cntr,Line_Cntr,Frame_Cntr);
}
