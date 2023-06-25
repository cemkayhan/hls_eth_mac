#include "hls_vid_dbg.h"
#include <iostream>

int main()
{
  hls::stream<ap_axiu<Axis_Vid_Busw<D_PIX_DEPTH_,D_PPC_>::Value,1,1,1>, 100> Vid_In ("Vid_In");
  hls::stream<ap_axiu<Axis_Vid_Busw<D_PIX_DEPTH_,D_PPC_>::Value,1,1,1>, 100> Vid_Out ("Vid_Out");
  ap_uint<16> Pixel_Cntr_Axi;
  ap_uint<16> Line_Cntr_Axi;
  ap_uint<16> Frame_Cntr_Axi;
  ap_uint<16> Pixel_Cntr;
  ap_uint<16> Line_Cntr;
  ap_uint<16> Frame_Cntr;

  for(auto i=0;i<100;++i){
    Vid_In.write(ap_axiu<Axis_Vid_Busw<D_PIX_DEPTH_,D_PPC_>::Value,1,1,1> {i,-1,-1,0,0,0,0});
  }
  
  for(auto i=0;i<100;++i){
    D_TOP_(Vid_In,Vid_Out,
           Pixel_Cntr_Axi,Line_Cntr_Axi,Frame_Cntr_Axi,
           &Pixel_Cntr,&Line_Cntr,&Frame_Cntr);
    std::cout << "i: " << i << ", " << Vid_Out.read().data << '\n';
  }
}
